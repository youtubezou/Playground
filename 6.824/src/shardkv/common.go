package shardkv

//
// Sharded key/value server.
// Lots of replica groups, each running op-at-a-time paxos.
// Shardmaster decides which group serves each shard.
// Shardmaster may change shard assignment from time to time.
//
// You will have to modify these definitions.
//
const (
	OK            = "OK"
	ErrNoKey      = "ErrNoKey"
	ErrWrongGroup = "ErrWrongGroup"
	ErrNotReady   = "ErrNotReady"
)

type Err string

type PutAppendArgs struct {
	Token     int64
	Key       string
	Value     string
	Op        string // "Put" or "Append"
	ConfigNum int
}

type PutAppendReply struct {
	Err Err
}

type GetArgs struct {
	Token     int64
	Key       string
	ConfigNum int
}

type GetReply struct {
	Err   Err
	Value string
}

type GetShardArgs struct {
	Token     int64
	Shard     int
	ConfigNum int
}

type GetShardReply struct {
	Err    Err
	Gets   map[int64]string
	Tokens map[int64]bool
	Values map[string]string
}
