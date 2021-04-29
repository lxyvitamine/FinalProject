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
using protos::Empty;
using protos::Request;
using protos::Response;

using namespace std;
using namespace grpc;
using namespace protos;

vector<string> parseCmd(const string &raw_line, const string &delim)
{
    vector<string> res;
    if (raw_line == "")
    {
#if PRINT
        cout << "empty string" << endl;
#endif
        return res;
    }
    else
    {
        char *strs = new char[raw_line.length() + 1];
        strcpy(strs, raw_line.c_str());

        char *d = new char[delim.length() + 1];
        strcpy(d, delim.c_str());

        char *p = strtok(strs, d);
        while (p)
        {
            string s = p;
            res.push_back(s);
            p = strtok(NULL, d);
        }

        delete[] strs;
        delete[] d;
        delete[] p;
        return res;
    }
}



void VoteClient::parseUserCmd(const std::string& input){
  Request request;
  request.set_userinput(input);
  
  Response response;
  ClientContext ctx;
  Status resp_code = this->stub->parseUserCmd(&ctx, request, &response);
  cout << "[C]: " << input <<endl;

  
  if(resp_code.ok()){
    
      cout<< response.feedback() << endl;
      
  }
  else {
    printf("%d: %s\n", resp_code.error_code(), resp_code.error_message().c_str());
  }
  return;
  
}

void VoteClient::shuttheserver(){
  
  Empty request;
  Response response;
  ClientContext ctx;

  
  Status resp_code = this->stub->shuttheserver(&ctx, request, &response);
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
  vector<string> inputs = parseCmd(command, " ");
  
        if(inputs[0] == "shutdown"){
        voter.shuttheserver();
      }


  return 0;
}