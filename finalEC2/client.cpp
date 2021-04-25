#include "client.h"
#include <grpcpp/grpcpp.h>
#include <cassert>
#include <cstdio>
#include <iostream>
#include <memory>
#include <sstream>
#include "finalec2.grpc.pb.h"

using grpc::ClientContext;
using grpc::Status;
//using protos::Empty;
using protos::Request;
//using protos::BookmarkList;
using protos::Response;
//using protos::BookmarkStatus;

using namespace std;
using namespace grpc;
using namespace protos;

void VoteClient::parseUserCmd(const std::string& input){
  Request request;
  request.set_userinput(input);
  
  Response response;
  ClientContext ctx;
  Status resp_code = this->stub->parseUserCmd(&ctx, request, &response);
  
  if(resp_code.ok()){
      cout<< response.feedback() << endl;
  }
  else{
    printf("%d: %s\n", resp_code.error_code(), resp_code.error_message().c_str());
  }
}

int main(int argc, char *argv[]){
      if (argc != 3)
    {
        cout << "Usage: ./client <port> “<command_name> <arg1> <arg2> ... <argN>”" << endl;
        return -1;
    }
  
  string port;
  port = argv[1];
  
  string command = argv[2];
  
  VoteClient voter(CreateChannel("localhost:" + port,
                                          InsecureChannelCredentials()));

  
  voter.parseUserCmd(command);

  return 0;
}