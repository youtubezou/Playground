package pbservice

const (
	OK             = "OK"
	ErrWrongServer = "ErrWrongServer"
	ErrBackup      = "ErrBackup"
)

type Err string

type PutAppendArgs struct {
	Key     string
	Value   string
	Append  bool
	Token   int64
	Primary string
	Version int
}

type PutAppendReply struct {
	Err Err
}

type GetArgs struct {
	Key string
}

type GetReply struct {
	Err   Err
	Value string
}

type ForwardArgs struct {
	Tokens   map[int64]bool
	Data     map[string]string
	Versions map[string]int
}

type ForwardReply struct {
	Err Err
}
