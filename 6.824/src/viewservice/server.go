package viewservice

import "net"
import "net/rpc"
import "log"
import "time"
import "sync"
import "fmt"
import "os"
import "sync/atomic"

type ViewServer struct {
	mu       sync.Mutex
	l        net.Listener
	dead     int32 // for testing
	rpccount int32 // for testing
	me       string

	// Your declarations here.
	ack             bool
	current         View // the view that acknowledged by the primary server
	candidate       View // the next view after current view is acknowledged
	primaryDeadPing int32
	backupDeadPing  int32
}

func (vs *ViewServer) PrepareBackup() {
	vs.candidate.Viewnum = vs.current.Viewnum + 1
	vs.candidate.Primary = vs.current.Backup
	vs.candidate.Backup = vs.current.Primary
}

//
// server Ping RPC handler.
//
func (vs *ViewServer) Ping(args *PingArgs, reply *PingReply) error {
	vs.mu.Lock()
	if vs.current.Viewnum == 0 {
		vs.current.Viewnum = 1
		vs.current.Primary = args.Me
		vs.primaryDeadPing = 0
		vs.candidate.Viewnum = vs.current.Viewnum
	} else if vs.current.Primary == args.Me {
		if args.Viewnum == 0 {
			vs.candidate.Viewnum = vs.current.Viewnum + 1
			vs.candidate.Primary = vs.current.Backup
			vs.candidate.Backup = args.Me
			vs.primaryDeadPing = vs.backupDeadPing
			vs.backupDeadPing = 0
			vs.ack = true
		} else {
			vs.primaryDeadPing = 0
			if args.Viewnum == vs.current.Viewnum {
				vs.ack = true
			}
		}
	} else if vs.current.Backup == "" {
		vs.candidate.Viewnum = vs.current.Viewnum + 1
		vs.candidate.Primary = vs.current.Primary
		vs.candidate.Backup = args.Me
		vs.backupDeadPing = 0
	} else if vs.current.Backup == args.Me {
		vs.backupDeadPing = 0
		if vs.primaryDeadPing >= DeadPings && args.Viewnum == vs.current.Viewnum {
			if vs.ack {
				vs.current.Viewnum = vs.current.Viewnum + 1
				vs.current.Primary = vs.current.Backup
				vs.current.Backup = ""
				vs.primaryDeadPing = 0
			}
		}
	}
	if vs.ack {
		if vs.candidate.Viewnum > vs.current.Viewnum {
			vs.current = vs.candidate
			vs.ack = false
		}
	}
	reply.View = vs.current
	vs.mu.Unlock()
	return nil
}

//
// server Get() RPC handler.
//
func (vs *ViewServer) Get(args *GetArgs, reply *GetReply) error {
	vs.mu.Lock()
	reply.View = vs.current
	vs.mu.Unlock()
	return nil
}

//
// tick() is called once per PingInterval; it should notice
// if servers have died or recovered, and change the view
// accordingly.
//
func (vs *ViewServer) tick() {
	vs.mu.Lock()
	vs.primaryDeadPing++
	if vs.current.Backup != "" {
		vs.backupDeadPing++
		if vs.backupDeadPing >= DeadPings {
			vs.candidate.Viewnum = vs.current.Viewnum + 1
			vs.candidate.Primary = vs.current.Primary
			vs.candidate.Backup = ""
		}
	}
	vs.mu.Unlock()
}

//
// tell the server to shut itself down.
// for testing.
// please don't change these two functions.
//
func (vs *ViewServer) Kill() {
	atomic.StoreInt32(&vs.dead, 1)
	vs.l.Close()
}

//
// has this server been asked to shut down?
//
func (vs *ViewServer) isdead() bool {
	return atomic.LoadInt32(&vs.dead) != 0
}

// please don't change this function.
func (vs *ViewServer) GetRPCCount() int32 {
	return atomic.LoadInt32(&vs.rpccount)
}

func StartServer(me string) *ViewServer {
	vs := new(ViewServer)
	vs.me = me
	// Your vs.* initializations here.

	// tell net/rpc about our RPC server and handlers.
	rpcs := rpc.NewServer()
	rpcs.Register(vs)

	// prepare to receive connections from clients.
	// change "unix" to "tcp" to use over a network.
	os.Remove(vs.me) // only needed for "unix"
	l, e := net.Listen("unix", vs.me)
	if e != nil {
		log.Fatal("listen error: ", e)
	}
	vs.l = l

	// please don't change any of the following code,
	// or do anything to subvert it.

	// create a thread to accept RPC connections from clients.
	go func() {
		for vs.isdead() == false {
			conn, err := vs.l.Accept()
			if err == nil && vs.isdead() == false {
				atomic.AddInt32(&vs.rpccount, 1)
				go rpcs.ServeConn(conn)
			} else if err == nil {
				conn.Close()
			}
			if err != nil && vs.isdead() == false {
				fmt.Printf("ViewServer(%v) accept: %v\n", me, err.Error())
				vs.Kill()
			}
		}
	}()

	// create a thread to call tick() periodically.
	go func() {
		for vs.isdead() == false {
			vs.tick()
			time.Sleep(PingInterval)
		}
	}()

	return vs
}
