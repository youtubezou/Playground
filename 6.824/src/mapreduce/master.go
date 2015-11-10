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
	fmt.Println("Deploying map")
	resultChan := make(chan bool)
	for i := 0; i < mr.nMap; i++ {
		go func(idx int) {
			worker := <- mr.registerChannel
			var arg DoJobArgs
			arg.File = mr.file
			arg.Operation = Map
			arg.JobNumber = idx
			arg.NumOtherPhase = mr.nReduce
			var reply DoJobReply
			call(worker, "Worker.DoJob", arg, &reply)
			resultChan <- true
			mr.registerChannel <- worker
		}(i)
	}
	fmt.Println("Mapping")
	for i := 0; i < mr.nMap; i++ {
		<-resultChan
	}
	fmt.Println("Finished")
	fmt.Println("Deploying reduce")
	for i := 0; i < mr.nReduce; i++ {
		go func(idx int) {
			worker := <- mr.registerChannel
			var arg DoJobArgs
			arg.File = mr.file
			arg.Operation = Reduce
			arg.JobNumber = idx
			arg.NumOtherPhase = mr.nMap
			var reply DoJobReply
			call(worker, "Worker.DoJob", arg, &reply)
			resultChan <- true
			mr.registerChannel <- worker
		}(i)
	}
	fmt.Println("Reducing")
	for i := 0; i < mr.nReduce; i++ {
		<-resultChan
	}
	fmt.Println("Finished")
	return mr.KillWorkers()
}
