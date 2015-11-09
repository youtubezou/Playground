package mapreduce

import "container/list"
import "fmt"

type WorkerInfo struct {
	address string
	// You can add definitions here.
}

// Clean up all workers by sending a Shutdown RPC to each one of them Collect
// the number of jobs each work has performed.
func (mr *MapReduce) KillWorkers() *list.List {
	l := list.New()
	for _, w := range mr.Workers {
		DPrintf("DoWork: shutdown %s\n", w.address)
		args := &ShutdownArgs{}
		var reply ShutdownReply
		ok := call(w.address, "Worker.Shutdown", args, &reply)
		if ok == false {
			fmt.Printf("DoWork: RPC %s shutdown error\n", w.address)
		} else {
			l.PushBack(reply.Njobs)
		}
	}
	return l
}

func (mr *MapReduce) RunMaster() *list.List {
	resultChan := make(chan bool)
	deployJob := func(idx int, op JobType, other int) {
		var arg DoJobArgs
		arg.File = mr.file
		arg.Operation = op
		arg.JobNumber = idx
		arg.NumOtherPhase = other
		var reply DoJobReply
		for {
			worker := <-mr.registerChannel
			result := call(worker, "Worker.DoJob", arg, &reply)
			if result && reply.OK {
				resultChan <- true
				mr.registerChannel <- worker
				break
			}
		}
	}
	fmt.Println("Deploying map")
	for i := 0; i < mr.nMap; i++ {
		go deployJob(i, Map, mr.nReduce)
	}
	fmt.Println("Waiting for map")
	for i := 0; i < mr.nMap; i++ {
		<-resultChan
	}
	fmt.Println("Deploying reduce")
	for i := 0; i < mr.nReduce; i++ {
		go deployJob(i, Reduce, mr.nMap)
	}
	fmt.Println("Waiting for reduce")
	for i := 0; i < mr.nReduce; i++ {
		<-resultChan
	}
	fmt.Println("Finished")
	return mr.KillWorkers()
}
