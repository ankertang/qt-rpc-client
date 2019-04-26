package main

import (
	"fmt"
	"net/http"
	"time"

	"github.com/gorilla/rpc"
	"github.com/gorilla/rpc/json"
)

// CallArgs define
type CallArgs struct {
	Who string
}

// CallReply define
type CallReply struct {
	Message string
}

// CallService define
type CallService struct{}

// Call func
func (h *CallService) Call(r *http.Request, args *CallArgs, reply *CallReply) error {
	reply.Message = "Hello, " + args.Who + "!"
	return nil
}

func main() {
	s := rpc.NewServer()
	s.RegisterCodec(json.NewCodec(), "application/json")
	s.RegisterService(new(CallService), "")
	//err := http.ListenAndServe(":888", s)
	err := http.ListenAndServeTLS(":888", "server.crt", "server.key", s)
	fmt.Println(err)

	for {
		time.Sleep(time.Second)
	}
}
