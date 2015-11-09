package kvpaxos

import "net"
import "fmt"
import "time"
import "net/rpc"
import "log"
import "paxos"
import "sync"
import "sync/atomic"
import "os"
import "syscall"
import "encoding/gob"
import "math/rand"

const Debug = 0

func DPrintf(format string, a ...interface{}) (n int, err error) {
	if Debug > 0 {
		log.Printf(format, a...)
	}
	return
}

type Op struct {
	Op    string
	Key   string
	Value string
	Token int64
}

type KVPaxos struct {
	mu         sync.Mutex
	l          net.Listener
	me         int
	dead       int32 // for testing
	unreliable int32 // for testing
	px         *paxos.Paxos

	confirmed int
	tokens    map[int64]bool
	values    map[string]string
}

func (kv *KVPaxos) ProcessValue(op Op) {
	kv.mu.Lock()
	defer kv.mu.Unlock()
	switch op.Op {
	case "Get":
		kv.tokens[op.Token] = true
	case "Put":
		kv.values[op.Key] = op.Value
		kv.tokens[op.Token] = true
	case "Append":
		kv.values[op.Key] = kv.values[op.Key] + op.Value
		kv.tokens[op.Token] = true
	case "Learn":
		fmt.Println("Fatal Error: Unexpected LEARN!!!", kv.confirmed, kv.px.Max())
		os.Exit(0)
	}
	kv.confirmed++
	kv.px.Done(kv.confirmed)
	//if len(op.Value) < 20 {fmt.Println("[Confirm] ", kv.me, kv.confirmed, op)}
}

func (kv *KVPaxos) CatchUp() {
	for {
		to := 10 * time.Millisecond
		for {
			if kv.confirmed < kv.px.Max() {
				break
			}
			time.Sleep(to)
			if to < 1*time.Second {
				to *= 2
			}
		}
		learning := false
		for {
			fate, value := kv.px.Status(kv.confirmed + 1)
			if fate == paxos.Decided {
				kv.ProcessValue(value.(Op))
				break
			} else {
				if !learning {
					var op Op
					op.Op = "Learn"
					kv.px.Start(kv.confirmed+1, op)
					learning = true
				}
			}
			time.Sleep(to)
			if to < 10*time.Second {
				to *= 2
			}
			//fmt.Println("[No Result] ", kv.me, kv.confirmed, "/", kv.px.Max())
		}
	}
}

func (kv *KVPaxos) Arise(op Op) bool {
	kv.mu.Lock()
	idx := kv.confirmed + 1
	kv.px.Start(idx, op)
	kv.mu.Unlock()
	to := 20 * time.Millisecond
	retry := 0
	for {
		fate, _ := kv.px.Status(idx)
		if fate == paxos.Decided || fate == paxos.Forgotten {
			break
		}
		retry++
		if retry >= 10 {
			return false
		}
		time.Sleep(to)
		to *= 2
	}
	return kv.tokens[op.Token]
}

func (kv *KVPaxos) Get(args *GetArgs, reply *GetReply) error {
	if kv.tokens[args.Token] {
		reply.Err = OK
		kv.mu.Lock()
		reply.Value = kv.values[args.Key]
		kv.mu.Unlock()
	} else {
		reply.Err = ErrNotReady
		if kv.confirmed == kv.px.Max() {
			var op Op
			op.Op = "Get"
			op.Key = args.Key
			op.Token = args.Token
			if kv.Arise(op) {
				reply.Err = OK
				kv.mu.Lock()
				reply.Value = kv.values[args.Key]
				kv.mu.Unlock()
			}
		}
	}
	return nil
}

func (kv *KVPaxos) PutAppend(args *PutAppendArgs, reply *PutAppendReply) error {
	if kv.tokens[args.Token] {
		reply.Err = OK
	} else {
		reply.Err = ErrNotReady
		if kv.confirmed == kv.px.Max() {
			var op Op
			op.Op = args.Op
			op.Key = args.Key
			op.Value = args.Value
			op.Token = args.Token
			if kv.Arise(op) {
				reply.Err = OK
			}
		}
	}
	return nil
}

// tell the server to shut itself down.
// please do not change these two functions.
func (kv *KVPaxos) kill() {
	DPrintf("Kill(%d): die\n", kv.me)
	atomic.StoreInt32(&kv.dead, 1)
	kv.l.Close()
	kv.px.Kill()
}

// call this to find out if the server is dead.
func (kv *KVPaxos) isdead() bool {
	return atomic.LoadInt32(&kv.dead) != 0
}

// please do not change these two functions.
func (kv *KVPaxos) setunreliable(what bool) {
	if what {
		atomic.StoreInt32(&kv.unreliable, 1)
	} else {
		atomic.StoreInt32(&kv.unreliable, 0)
	}
}

func (kv *KVPaxos) isunreliable() bool {
	return atomic.LoadInt32(&kv.unreliable) != 0
}

//
// servers[] contains the ports of the set of
// servers that will cooperate via Paxos to
// form the fault-tolerant key/value service.
// me is the index of the current server in servers[].
//
func StartServer(servers []string, me int) *KVPaxos {
	// call gob.Register on structures you want
	// Go's RPC library to marshall/unmarshall.
	gob.Register(Op{})

	kv := new(KVPaxos)
	kv.me = me

	kv.confirmed = -1
	kv.tokens = make(map[int64]bool)
	kv.values = make(map[string]string)

	rpcs := rpc.NewServer()
	rpcs.Register(kv)

	kv.px = paxos.Make(servers, me, rpcs)
	go kv.CatchUp()

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
				fmt.Printf("KVPaxos(%v) accept: %v\n", me, err.Error())
				kv.kill()
			}
		}
	}()

	return kv
}
