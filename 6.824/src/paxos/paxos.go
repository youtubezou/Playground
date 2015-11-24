package paxos

//
// Paxos library, to be included in an application.
// Multiple applications will run, each including
// a Paxos peer.
//
// Manages a sequence of agreed-on values.
// The set of peers is fixed.
// Copes with network failures (partition, msg loss, &c).
// Does not store anything persistently, so cannot handle crash+restart.
//
// The application interface:
//
// px = paxos.Make(peers []string, me string)
// px.Start(seq int, v interface{}) -- start agreement on new instance
// px.Status(seq int) (Fate, v interface{}) -- get info about an instance
// px.Done(seq int) -- ok to forget all instances <= seq
// px.Max() int -- highest instance seq known, or -1
// px.Min() int -- instances before this seq have been forgotten
//

import "net"
import "net/rpc"
import "log"

import "os"
import "syscall"
import "sync"
import "sync/atomic"
import "fmt"
import "math/rand"

// px.Status() return values, indicating
// whether an agreement has been decided,
// or Paxos has not yet reached agreement,
// or it was agreed but forgotten (i.e. < Min()).
type Fate int

const (
	Decided   Fate = iota + 1
	Pending        // not yet decided.
	Forgotten      // decided but forgotten.
)

type Paxos struct {
	mu         sync.Mutex
	l          net.Listener
	dead       int32 // for testing
	unreliable int32 // for testing
	rpcCount   int32 // for testing
	peers      []string
	me         int // index into peers[]

	fates   map[int]Fate
	seens   map[int]int
	accepts map[int]int
	values  map[int]interface{}
	dones   []int
	highest int
	maxSeen int
}

//
// call() sends an RPC to the rpcname handler on server srv
// with arguments args, waits for the reply, and leaves the
// reply in reply. the reply argument should be a pointer
// to a reply structure.
//
// the return value is true if the server responded, and false
// if call() was not able to contact the server. in particular,
// the replys contents are only valid if call() returned true.
//
// you should assume that call() will time out and return an
// error after a while if it does not get a reply from the server.
//
// please use call() to send all RPCs, in client.go and server.go.
// please do not change this function.
//
func call(srv string, name string, args interface{}, reply interface{}) bool {
	c, err := rpc.Dial("unix", srv)
	if err != nil {
		err1 := err.(*net.OpError)
		if err1.Err != syscall.ENOENT && err1.Err != syscall.ECONNREFUSED {
			fmt.Printf("paxos Dial() failed: %v\n", err1)
		}
		return false
	}
	defer c.Close()

	err = c.Call(name, args, reply)
	if err == nil {
		return true
	}

	fmt.Println(err)
	return false
}

type PrepareArgs struct {
	Seq int
	Num int
}

type PrepareReply struct {
	Num   int
	Value interface{}
}

func (px *Paxos) Prepare(args *PrepareArgs, reply *PrepareReply) error {
	px.mu.Lock()
	defer px.mu.Unlock()
	if args.Num > px.accepts[args.Seq] {
		px.accepts[args.Seq] = args.Num
	}
	reply.Num = px.seens[args.Seq]
	reply.Value = px.values[args.Seq]
	//fmt.Println("[Prepare] ", px.me, px.accepts[args.Seq], args, reply)
	return nil
}

type AcceptArgs struct {
	Seq   int
	Num   int
	Value interface{}
}

type AcceptReply struct {
	Seq int
}

func (px *Paxos) Accept(args *AcceptArgs, reply *AcceptReply) error {
	px.mu.Lock()
	defer px.mu.Unlock()
	if args.Num >= px.accepts[args.Seq] {
		px.accepts[args.Seq] = args.Num
		px.seens[args.Seq] = args.Num
		px.values[args.Seq] = args.Value
	}
	reply.Seq = px.accepts[args.Seq]
	//fmt.Println("[Accept] ", px.me, args, reply)
	return nil
}

type DecidedArgs struct {
	Seq   int
	Num   int
	Value interface{}
}

type DecidedReply struct {
}

func (px *Paxos) Decided(args *DecidedArgs, reply *DecidedReply) error {
	px.mu.Lock()
	defer px.mu.Unlock()
	if args.Num > px.accepts[args.Seq] {
		px.accepts[args.Seq] = args.Num
	}
	px.seens[args.Seq] = args.Num
	px.values[args.Seq] = args.Value
	px.fates[args.Seq] = Decided
	if args.Seq > px.highest {
		px.highest = args.Seq
	}
	//fmt.Println("[Decided] ", px.me, args, reply)
	return nil
}

func (px *Paxos) Propose(seq int, v interface{}) {
	//fmt.Println("[Propose] ", px.me, seq, v);
	px.mu.Lock()
	if px.seens[seq] == 0 {
		px.accepts[seq] = px.me
		px.seens[seq] = px.me
	}
	px.fates[seq] = Pending
	px.mu.Unlock()
	for {
		px.mu.Lock()
		px.maxSeen += 1000
		n := px.maxSeen
		px.mu.Unlock()
		var prepareArgs PrepareArgs
		prepareArgs.Seq = seq
		prepareArgs.Num = n
		existNum := -1
		connNum := 0
		for _, peer := range px.peers {
			var reply PrepareReply
			ok := call(peer, "Paxos.Prepare", &prepareArgs, &reply)
			if !ok {
				continue
			}
			if reply.Num > existNum && reply.Value != nil {
				v = reply.Value
				existNum = reply.Num
			}
			px.mu.Lock()
			for reply.Num >= px.maxSeen {
				px.maxSeen += 1000
			}
			px.mu.Unlock()
			connNum++
			if connNum >= len(px.peers)/2+1 {
				break
			}
		}
		if connNum < len(px.peers)/2+1 {
			continue
		}
		var acceptArgs AcceptArgs
		acceptArgs.Seq = seq
		acceptArgs.Num = n
		acceptArgs.Value = v
		accepted := true
		connNum = 0
		for _, peer := range px.peers {
			var reply AcceptReply
			ok := call(peer, "Paxos.Accept", &acceptArgs, &reply)
			if !ok {
				continue
			}
			if reply.Seq > n {
				accepted = false
				px.mu.Lock()
				for reply.Seq > px.maxSeen {
					px.maxSeen += 1000
				}
				px.mu.Unlock()
			}
			connNum++
			if connNum >= len(px.peers)/2+1 {
				break
			}
		}
		if accepted && connNum >= len(px.peers)/2+1 {
			var decidedArgs DecidedArgs
			decidedArgs.Seq = seq
			decidedArgs.Num = n
			decidedArgs.Value = v
			decidedReplies := make(chan DecidedReply)
			for idx, peer := range px.peers {
				if idx == px.me {
					continue
				}
				go func(peer string) {
					var reply DecidedReply
					call(peer, "Paxos.Decided", &decidedArgs, &reply)
					decidedReplies <- reply
				}(peer)
			}
			for i := 1; i < len(px.peers); i++ {
				<-decidedReplies
			}
			px.mu.Lock()
			px.accepts[seq] = n
			px.seens[seq] = n
			px.values[seq] = v
			px.fates[seq] = Decided
			if seq > px.highest {
				px.highest = seq
			}
			//fmt.Println("[Decided] ", px.me, seq, v)
			px.mu.Unlock()
			break
		}
	}
	//fmt.Println("[Finished] ", px.me, px.seens[seq], px.values[seq])
}

//
// the application wants paxos to start agreement on
// instance seq, with proposed value v.
// Start() returns right away; the application will
// call Status() to find out if/when agreement
// is reached.
//
func (px *Paxos) Start(seq int, v interface{}) {
	go px.Propose(seq, v)
}

type PeerDoneArgs struct {
	Me  int
	Seq int
}

type PeerDoneReply struct {
}

func (px *Paxos) NoLockPeerDone(idx int, seq int) {
	lastMin := px.NoLockMin()
	px.dones[idx] = seq + 1
	curMin := px.NoLockMin()
	for i := lastMin; i < curMin; i++ {
		delete(px.fates, i)
		delete(px.accepts, i)
		delete(px.seens, i)
		delete(px.values, i)
	}
}

func (px *Paxos) PeerDone(args *PeerDoneArgs, reply *PeerDoneReply) error {
	px.mu.Lock()
	defer px.mu.Unlock()
	px.NoLockPeerDone(args.Me, args.Seq)
	return nil
}

//
// the application on this machine is done with
// all instances <= seq.
//
// see the comments for Min() for more explanation.
//
func (px *Paxos) Done(seq int) {
	var args PeerDoneArgs
	args.Me = px.me
	args.Seq = seq
	for idx, peer := range px.peers {
		if idx == px.me {
			continue
		}
		go func(peer string) {
			var reply PeerDoneReply
			call(peer, "Paxos.PeerDone", &args, &reply)
		}(peer)
	}
	px.mu.Lock()
	px.NoLockPeerDone(px.me, seq)
	px.mu.Unlock()
}

//
// the application wants to know the
// highest instance sequence known to
// this peer.
//
func (px *Paxos) Max() int {
	return px.highest
}

func (px *Paxos) NoLockMin() int {
	minSeq := px.dones[0]
	for i := 1; i < len(px.peers); i++ {
		if px.dones[i] < minSeq {
			minSeq = px.dones[i]
		}
	}
	return minSeq
}

//
// Min() should return one more than the minimum among z_i,
// where z_i is the highest number ever passed
// to Done() on peer i. A peers z_i is -1 if it has
// never called Done().
//
// Paxos is required to have forgotten all information
// about any instances it knows that are < Min().
// The point is to free up memory in long-running
// Paxos-based servers.
//
// Paxos peers need to exchange their highest Done()
// arguments in order to implement Min(). These
// exchanges can be piggybacked on ordinary Paxos
// agreement protocol messages, so it is OK if one
// peers Min does not reflect another Peers Done()
// until after the next instance is agreed to.
//
// The fact that Min() is defined as a minimum over
// *all* Paxos peers means that Min() cannot increase until
// all peers have been heard from. So if a peer is dead
// or unreachable, other peers Min()s will not increase
// even if all reachable peers call Done. The reason for
// this is that when the unreachable peer comes back to
// life, it will need to catch up on instances that it
// missed -- the other peers therefor cannot forget these
// instances.
//
func (px *Paxos) Min() int {
	px.mu.Lock()
	defer px.mu.Unlock()
	return px.NoLockMin()
}

//
// the application wants to know whether this
// peer thinks an instance has been decided,
// and if so what the agreed value is. Status()
// should just inspect the local peer state;
// it should not contact other Paxos peers.
//
func (px *Paxos) Status(seq int) (Fate, interface{}) {
	px.mu.Lock()
	defer px.mu.Unlock()
	if seq < px.NoLockMin() {
		return Forgotten, nil
	}
	if _, ok := px.fates[seq]; !ok {
		px.fates[seq] = Pending
	}
	return px.fates[seq], px.values[seq]
}

//
// tell the peer to shut itself down.
// for testing.
// please do not change these two functions.
//
func (px *Paxos) Kill() {
	atomic.StoreInt32(&px.dead, 1)
	if px.l != nil {
		px.l.Close()
	}
}

//
// has this peer been asked to shut down?
//
func (px *Paxos) isdead() bool {
	return atomic.LoadInt32(&px.dead) != 0
}

// please do not change these two functions.
func (px *Paxos) setunreliable(what bool) {
	if what {
		atomic.StoreInt32(&px.unreliable, 1)
	} else {
		atomic.StoreInt32(&px.unreliable, 0)
	}
}

func (px *Paxos) isunreliable() bool {
	return atomic.LoadInt32(&px.unreliable) != 0
}

//
// the application wants to create a paxos peer.
// the ports of all the paxos peers (including this one)
// are in peers[]. this servers port is peers[me].
//
func Make(peers []string, me int, rpcs *rpc.Server) *Paxos {
	px := &Paxos{}
	px.peers = peers
	px.me = me

	fmt.Println("Peer Number: ", len(peers))

	px.fates = make(map[int]Fate)
	px.seens = make(map[int]int)
	px.accepts = make(map[int]int)
	px.values = make(map[int]interface{})
	px.dones = make([]int, len(peers))
	px.highest = -1
	px.maxSeen = px.me

	if rpcs != nil {
		// caller will create socket &c
		rpcs.Register(px)
	} else {
		rpcs = rpc.NewServer()
		rpcs.Register(px)

		// prepare to receive connections from clients.
		// change "unix" to "tcp" to use over a network.
		os.Remove(peers[me]) // only needed for "unix"
		l, e := net.Listen("unix", peers[me])
		if e != nil {
			log.Fatal("listen error: ", e)
		}
		px.l = l

		// please do not change any of the following code,
		// or do anything to subvert it.

		// create a thread to accept RPC connections
		go func() {
			for px.isdead() == false {
				conn, err := px.l.Accept()
				if err == nil && px.isdead() == false {
					if px.isunreliable() && (rand.Int63()%1000) < 100 {
						// discard the request.
						conn.Close()
					} else if px.isunreliable() && (rand.Int63()%1000) < 200 {
						// process the request but force discard of reply.
						c1 := conn.(*net.UnixConn)
						f, _ := c1.File()
						err := syscall.Shutdown(int(f.Fd()), syscall.SHUT_WR)
						if err != nil {
							fmt.Printf("shutdown: %v\n", err)
						}
						atomic.AddInt32(&px.rpcCount, 1)
						go rpcs.ServeConn(conn)
					} else {
						atomic.AddInt32(&px.rpcCount, 1)
						go rpcs.ServeConn(conn)
					}
				} else if err == nil {
					conn.Close()
				}
				if err != nil && px.isdead() == false {
					fmt.Printf("Paxos(%v) accept: %v\n", me, err.Error())
				}
			}
		}()
	}

	return px
}
