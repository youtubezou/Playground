package shardkv

import "net"
import "fmt"
import "net/rpc"
import "log"
import "time"
import "paxos"
import "sync"
import "sync/atomic"
import "os"
import "syscall"
import "encoding/gob"
import "math/rand"
import "shardmaster"

const Debug = 0

func DPrintf(format string, a ...interface{}) (n int, err error) {
	if Debug > 0 {
		log.Printf(format, a...)
	}
	return
}

const (
	OpGet      = "Get"
	OpPut      = "Put"
	OpAppend   = "Append"
	OpReconfig = "Reconfig"
	OpGetShard = "GetShard"
)

type Op struct {
	Type      string
	Token     int64
	Key       string
	Value     string
	Shard     int
	ConfigNum int
	Tokens    map[int64]bool
	Values    map[string]string
}

type ShardKV struct {
	mu         sync.Mutex
	l          net.Listener
	me         int
	dead       int32 // for testing
	unreliable int32 // for testing
	sm         *shardmaster.Clerk
	px         *paxos.Paxos

	gid int64 // my replica group ID

	tokens    map[int64]bool
	values    map[string]string
	confirmed int
	config    shardmaster.Config
}

func (kv *ShardKV) AriseOp(op Op) bool {
	kv.px.Start(kv.confirmed+1, op)
	to := 10 * time.Millisecond
	retry := 0
	for {
		fate, value := kv.px.Status(kv.confirmed + 1)
		if fate == paxos.Decided {
			kv.ConfirmOp(value.(Op))
			return value.(Op).Token == op.Token
		}
		time.Sleep(to)
		if to < 10*time.Second {
			to *= 2
		}
		retry++
		if retry >= 10 {
			break
		}
	}
	return false
}

func (kv *ShardKV) ConfirmOp(op Op) {
	fmt.Println("["+op.Type+"]", kv.confirmed, kv.gid, "-", kv.me, kv.config)
	switch op.Type {
	case OpGet:
		if kv.config.Num == op.ConfigNum && kv.config.Shards[key2shard(op.Key)] == kv.gid {
			kv.tokens[op.Token] = true
		}
		fmt.Println("Key =", op.Key, kv.config.Shards[op.Shard], kv.gid)
	case OpPut:
		if kv.config.Num == op.ConfigNum && kv.config.Shards[key2shard(op.Key)] == kv.gid {
			if _, exist := kv.tokens[op.Token]; !exist {
				kv.tokens[op.Token] = true
				kv.values[op.Key] = op.Value
			}
		}
		fmt.Println("Key =", op.Key, " Value =", op.Value, kv.config.Shards[op.Shard], kv.gid)
	case OpAppend:
		if kv.config.Num == op.ConfigNum && kv.config.Shards[key2shard(op.Key)] == kv.gid {
			if _, exist := kv.tokens[op.Token]; !exist {
				kv.tokens[op.Token] = true
				kv.values[op.Key] = kv.values[op.Key] + op.Value
			}
		}
		fmt.Println("Key =", op.Key, " Value =", op.Value, kv.config.Shards[op.Shard], kv.gid)
	case OpReconfig:
		if op.ConfigNum == kv.config.Num+1 {
			kv.config = kv.sm.Query(op.ConfigNum)
			for token := range op.Tokens {
				kv.tokens[token] = true
			}
			for key, value := range op.Values {
				kv.values[key] = value
			}
			kv.tokens[op.Token] = true
		}
		fmt.Println("Config Num =", kv.config.Num, " New Num =", op.ConfigNum, " Values =", op.Values)
	case OpGetShard:
		kv.tokens[op.Token] = true
		fmt.Println("Config Num =", op.ConfigNum, " Shard =", op.Shard)
	}
	fmt.Println()
	kv.confirmed++
	kv.px.Done(kv.confirmed)
}

func (kv *ShardKV) Get(args *GetArgs, reply *GetReply) error {
	kv.mu.Lock()
	defer kv.mu.Unlock()
	if kv.tokens[args.Token] {
		reply.Err = OK
		reply.Value = kv.values[args.Key]
		return nil
	}
	var op Op
	op.Type = OpGet
	op.Token = args.Token
	op.Key = args.Key
	op.ConfigNum = args.ConfigNum
	if kv.AriseOp(op) {
		if kv.tokens[args.Token] {
			reply.Err = OK
			reply.Value = kv.values[args.Key]
		} else {
			reply.Err = ErrWrongGroup
		}
	} else {
		reply.Err = ErrNotReady
	}
	return nil
}

// RPC handler for client Put and Append requests
func (kv *ShardKV) PutAppend(args *PutAppendArgs, reply *PutAppendReply) error {
	kv.mu.Lock()
	defer kv.mu.Unlock()
	if kv.tokens[args.Token] {
		reply.Err = OK
		return nil
	}
	var op Op
	if args.Op == "Put" {
		op.Type = OpPut
	} else {
		op.Type = OpAppend
	}
	op.Token = args.Token
	op.Key = args.Key
	op.Value = args.Value
	op.ConfigNum = args.ConfigNum
	if kv.AriseOp(op) {
		if kv.tokens[args.Token] {
			reply.Err = OK
		} else {
			reply.Err = ErrWrongGroup
		}
	} else {
		reply.Err = ErrNotReady
	}
	return nil
}

func (kv *ShardKV) GetShard(args *GetShardArgs, reply *GetShardReply) error {
	kv.mu.Lock()
	defer kv.mu.Unlock()
	if args.ConfigNum > kv.config.Num {
		reply.Err = ErrNotReady
	}
	var op Op
	op.Type = OpGetShard
	op.Token = args.Token
	op.Shard = args.Shard
	op.ConfigNum = args.ConfigNum
	fmt.Println("Arise get shard")
	if kv.AriseOp(op) {
		reply.Err = OK
		reply.Tokens = kv.tokens
		reply.Values = make(map[string]string)
		for key, value := range kv.values {
			if key2shard(key) == args.Shard {
				reply.Values[key] = value
			}
		}
	} else {
		reply.Err = ErrNotReady
	}
	return nil
}

//
// Ask the shardmaster if there's a new configuration;
// if so, re-configure.
//
func (kv *ShardKV) tick() {
	kv.mu.Lock()
	defer kv.mu.Unlock()
	fmt.Println("[Tick]", kv.gid, kv.me, kv.config.Num, "/", kv.sm.Query(-1).Num, kv.confirmed, "/", kv.px.Max())
	if kv.confirmed < kv.px.Max() {
		fate, value := kv.px.Status(kv.confirmed + 1)
		fmt.Println(paxos.Decided, fate, value)
		if fate == paxos.Decided {
			kv.ConfirmOp(value.(Op))
		}
		return
	}
	fmt.Println("[-Tick-]")
	newConfig := kv.sm.Query(kv.config.Num + 1)
	if newConfig.Num == kv.config.Num+1 {
		var op Op
		op.Type = OpReconfig
		op.Token = nrand()
		op.Tokens = make(map[int64]bool)
		op.Values = make(map[string]string)
		op.ConfigNum = newConfig.Num
		for i := 0; i < shardmaster.NShards; i++ {
			lastGid := kv.config.Shards[i]
			if lastGid != 0 && lastGid != kv.gid && newConfig.Shards[i] == kv.gid {
				var args GetShardArgs
				args.Token = nrand()
				args.Shard = i
				args.ConfigNum = newConfig.Num
				var reply GetShardReply
				responsed := false
				fmt.Println("Shard: ", i)
				for _, server := range kv.config.Groups[kv.config.Shards[i]] {
					fmt.Println("Shard Server: ", i, lastGid, i)
					ok := call(server, "ShardKV.GetShard", &args, &reply)
					fmt.Println("Shard Server: ", i, server, reply)
					if ok && reply.Err == OK {
						responsed = true
						break
					}
				}
				if responsed {
					for token := range reply.Tokens {
						op.Tokens[token] = true
					}
					for key, value := range reply.Values {
						op.Values[key] = value
					}
				} else {
					fmt.Println("Can not get", i)
					return
				}
			}
		}
		fmt.Println("Try reconfig")
		kv.AriseOp(op)
	}
	fmt.Println("[[Tick]]", kv.gid, kv.me)
}

// tell the server to shut itself down.
// please don't change these two functions.
func (kv *ShardKV) kill() {
	atomic.StoreInt32(&kv.dead, 1)
	kv.l.Close()
	kv.px.Kill()
}

// call this to find out if the server is dead.
func (kv *ShardKV) isdead() bool {
	return atomic.LoadInt32(&kv.dead) != 0
}

// please do not change these two functions.
func (kv *ShardKV) Setunreliable(what bool) {
	if what {
		atomic.StoreInt32(&kv.unreliable, 1)
	} else {
		atomic.StoreInt32(&kv.unreliable, 0)
	}
}

func (kv *ShardKV) isunreliable() bool {
	return atomic.LoadInt32(&kv.unreliable) != 0
}

//
// Start a shardkv server.
// gid is the ID of the server's replica group.
// shardmasters[] contains the ports of the
//   servers that implement the shardmaster.
// servers[] contains the ports of the servers
//   in this replica group.
// Me is the index of this server in servers[].
//
func StartServer(gid int64, shardmasters []string,
	servers []string, me int) *ShardKV {
	gob.Register(Op{})

	kv := new(ShardKV)
	kv.me = me
	kv.gid = gid
	kv.sm = shardmaster.MakeClerk(shardmasters)

	kv.tokens = make(map[int64]bool)
	kv.values = make(map[string]string)
	kv.confirmed = -1
	kv.config = kv.sm.Query(0)

	rpcs := rpc.NewServer()
	rpcs.Register(kv)

	kv.px = paxos.Make(servers, me, rpcs)

	os.Remove(servers[me])
	l, e := net.Listen("unix", servers[me])
	if e != nil {
		log.Fatal("listen error: ", e)
	}
	kv.l = l

	// please do not change any of the following code,
	// or do anything to subvert it.

	go func() {
		for kv.isdead() == false {
			conn, err := kv.l.Accept()
			if err == nil && kv.isdead() == false {
				if kv.isunreliable() && (rand.Int63()%1000) < 100 {
					// discard the request.
					conn.Close()
				} else if kv.isunreliable() && (rand.Int63()%1000) < 200 {
					// process the request but force discard of reply.
					c1 := conn.(*net.UnixConn)
					f, _ := c1.File()
					err := syscall.Shutdown(int(f.Fd()), syscall.SHUT_WR)
					if err != nil {
						fmt.Printf("shutdown: %v\n", err)
					}
					go rpcs.ServeConn(conn)
				} else {
					go rpcs.ServeConn(conn)
				}
			} else if err == nil {
				conn.Close()
			}
			if err != nil && kv.isdead() == false {
				fmt.Printf("ShardKV(%v) accept: %v\n", me, err.Error())
				kv.kill()
			}
		}
	}()

	go func() {
		for kv.isdead() == false {
			kv.tick()
			time.Sleep(250 * time.Millisecond)
		}
		fmt.Println("[DEAD]", kv.gid, kv.me)
	}()

	return kv
}
