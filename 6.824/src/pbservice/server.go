package pbservice

import "net"
import "fmt"
import "net/rpc"
import "log"
import "time"
import "viewservice"
import "sync"
import "sync/atomic"
import "os"
import "syscall"
import "math/rand"

type PBServer struct {
	mu         sync.Mutex
	l          net.Listener
	dead       int32 // for testing
	unreliable int32 // for testing
	me         string
	vs         *viewservice.Clerk
	// Your declarations here.
	view      viewservice.View
	forwarded bool
	tokens    map[int64]bool
	data      map[string]string
	versions  map[string]int
}

func (pb *PBServer) Get(args *GetArgs, reply *GetReply) error {
	pb.mu.Lock()
	defer pb.mu.Unlock()
	if pb.view.Primary != pb.me {
		reply.Err = ErrWrongServer
	} else {
		reply.Err = OK
		reply.Value = pb.data[args.Key]
	}
	// fmt.Println("Get", args, reply)
	return nil
}

func (pb *PBServer) PutAppend(args *PutAppendArgs, reply *PutAppendReply) error {
	pb.mu.Lock()
	defer pb.mu.Unlock()
	if args.Primary == "" && pb.view.Primary != pb.me {
		reply.Err = ErrWrongServer
	} else if args.Primary != "" && (pb.view.Primary != args.Primary || pb.view.Backup != pb.me) {
		reply.Err = ErrWrongServer
	} else if pb.tokens[args.Token] {
		reply.Err = OK
	} else {
		ok := true
		if pb.view.Primary == pb.me && pb.view.Backup != "" && pb.forwarded {
			var backupArgs PutAppendArgs
			backupArgs = *args
			backupArgs.Primary = pb.me
			backupArgs.Version = pb.versions[args.Key]
			var backupReply PutAppendReply
			ok = call(pb.view.Backup, "PBServer.PutAppend", &backupArgs, &backupReply)
			if !ok || backupReply.Err != OK {
				reply.Err = ErrBackup
				ok = false
			}
		}
		if pb.view.Backup == pb.me {
			if args.Version != pb.versions[args.Key] {
				if args.Version+1 == pb.versions[args.Key] {
					if args.Value != pb.data[args.Key] {
						ok = false
					} else {
						pb.versions[args.Key] = pb.versions[args.Key] - 1
					}
				} else {
					ok = false
				}
			}
		}
		if ok {
			pb.tokens[args.Token] = true
			pb.versions[args.Key] = pb.versions[args.Key] + 1
			reply.Err = OK
			if args.Append {
				pb.data[args.Key] = pb.data[args.Key] + args.Value
			} else {
				pb.data[args.Key] = args.Value
			}
		}
	}
	// fmt.Println("PutAppend", args, reply)
	return nil
}

func (pb *PBServer) Forward(args *ForwardArgs, reply *ForwardReply) error {
	pb.mu.Lock()
	defer pb.mu.Unlock()
	if pb.view.Backup != pb.me {
		reply.Err = ErrWrongServer
	} else {
		pb.tokens = args.Tokens
		pb.data = args.Data
		pb.versions = args.Versions
		reply.Err = OK
	}
	// fmt.Println("Receive", args, reply)
	return nil
}

//
// ping the viewserver periodically.
// if view changed:
//   transition to new view.
//   manage transfer of state from primary to new backup.
//
func (pb *PBServer) tick() {
	pb.mu.Lock()
	defer pb.mu.Unlock()
	view, err := pb.vs.Ping(pb.view.Viewnum)
	if err == nil {
		if pb.me == pb.view.Primary {
			if view.Backup != "" && view.Backup != pb.view.Backup {
				pb.forwarded = false
			}
		}
	}
	pb.view = view
	if pb.me == pb.view.Primary && !pb.forwarded {
		if pb.view.Backup != "" {
			var args ForwardArgs
			args.Tokens = pb.tokens
			args.Data = pb.data
			args.Versions = pb.versions
			var reply ForwardReply
			ok := call(pb.view.Backup, "PBServer.Forward", &args, &reply)
			if ok && reply.Err == OK {
				pb.forwarded = true
			}
		}
	}
	// fmt.Println("Tick", pb.me, pb.view)
}

// tell the server to shut itself down.
// please do not change these two functions.
func (pb *PBServer) kill() {
	atomic.StoreInt32(&pb.dead, 1)
	pb.l.Close()
}

// call this to find out if the server is dead.
func (pb *PBServer) isdead() bool {
	return atomic.LoadInt32(&pb.dead) != 0
}

// please do not change these two functions.
func (pb *PBServer) setunreliable(what bool) {
	if what {
		atomic.StoreInt32(&pb.unreliable, 1)
	} else {
		atomic.StoreInt32(&pb.unreliable, 0)
	}
}

func (pb *PBServer) isunreliable() bool {
	return atomic.LoadInt32(&pb.unreliable) != 0
}

func StartServer(vshost string, me string) *PBServer {
	pb := new(PBServer)
	pb.me = me
	pb.vs = viewservice.MakeClerk(me, vshost)
	pb.forwarded = true
	pb.tokens = make(map[int64]bool)
	pb.data = make(map[string]string)
	pb.versions = make(map[string]int)

	rpcs := rpc.NewServer()
	rpcs.Register(pb)

	os.Remove(pb.me)
	l, e := net.Listen("unix", pb.me)
	if e != nil {
		log.Fatal("listen error: ", e)
	}
	pb.l = l

	// please do not change any of the following code,
	// or do anything to subvert it.

	go func() {
		for pb.isdead() == false {
			conn, err := pb.l.Accept()
			if err == nil && pb.isdead() == false {
				if pb.isunreliable() && (rand.Int63()%1000) < 100 {
					// discard the request.
					conn.Close()
				} else if pb.isunreliable() && (rand.Int63()%1000) < 200 {
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
			if err != nil && pb.isdead() == false {
				fmt.Printf("PBServer(%v) accept: %v\n", me, err.Error())
				pb.kill()
			}
		}
	}()

	go func() {
		for pb.isdead() == false {
			pb.tick()
			time.Sleep(viewservice.PingInterval)
		}
	}()

	return pb
}
