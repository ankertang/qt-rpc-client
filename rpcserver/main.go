package main

import (
	"log"
	"net/http"
	"time"

	"github.com/gorilla/rpc"
	"github.com/gorilla/rpc/json"
	"github.com/thomas92911/go-utils"
)

// GraphQL
// https://graphql.cn/code/#go
//
// MacOS Rest Client
// https://github.com/mmattozzi/cocoa-rest-client/releases
//
// openssl genrsa -out server.key 2048
// openssl req -nodes -new -key server.key -subj "/CN=localhost" -out server.csr
// openssl x509 -req -sha256 -days 888888 -in server.csr -signkey server.key -out server.crt

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
	//return errors.New("error haha")
}

func init() {
	bs, err := json.EncodeClientRequest("CallService.Call", &CallArgs{"Merak"})
	log.Println(string(bs), err)
	// {"method":"CallService.Call","params":[{"Who":"Merak"}],"id":5577006791947779410}

	s := rpc.NewServer()
	s.RegisterCodec(json.NewCodec(), "application/json")
	s.RegisterService(new(CallService), "")
	//err = http.ListenAndServe(":888", s)
	err = http.ListenAndServeTLS(":888", "server.crt", "server.key", s)
	log.Println(err)
}

func main() {

	port, err := utils.GetUnUsedPort()
	log.Println(port, err)

	for {
		time.Sleep(time.Second)
	}
}
