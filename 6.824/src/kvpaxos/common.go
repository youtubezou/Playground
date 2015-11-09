package kvpaxos

const (
	OK          = "OK"
	ErrNotReady = "ErrNotReady"
)

type Err string

// Put or Append
type PutAppendArgs struct {
	// You'll have to add definitions here.
	Key   string
	Value string
	Op    string // "Put" or "Append"
	Token int64
}

type PutAppendReply struct {
	Err Err
}

type GetArgs struct {
	Key   string
	Token int64
}

type GetReply struct {
	Err   Err
	Value string
}
