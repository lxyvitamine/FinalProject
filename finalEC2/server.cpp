#include "server.h"
#include "finalec2.grpc.pb.h"
#include <grpcpp/grpcpp.h>
#include <iostream>
#include <memory>
#include <string>
#include <cstdio>
using namespace std;
using namespace grpc;
using namespace protos;

string server_response;

Status VoteServer::parseUserCmd(ServerContext* context, const Request* request, Response* response){
  string input = request->userinput().c_str();
  
  vector<string> inputs = parseCmd(input, " ");
  
    // no argument
    if (inputs.size() == 1)
    {
        if (inputs[0] == "list_candidates")
        {
            server_response = list_candidtates();

        }
        else if (inputs[0] == "view_result")
        {
            server_response = view_result();

        }
        else
        {
              server_response = errorMsg;
        }
    }
    // 1 argument
    else if (inputs.size() == 2)
    {
        string cmd = inputs[0];
        string arg = inputs[1];

        if (cmd == "start_election")
        {

              server_response = start_election(arg);

        }
        else if (cmd == "end_election")
        {

              server_response = end_election(arg);

        }
        else if (cmd == "shutdown")
        {
            server_response = shutdown(arg);
            response->set_feedback(server_response);
            exit(0);
        }
        else if (cmd == "add_voter")
        {
            if (!isNumber(arg))
            {
                  server_response =errorMsg;
                   return Status::CANCELLED;
            }
           
              server_response =add_voter(stoi(arg));
        }
        else if (cmd == "check_registration_status")
        {
            if (!isNumber(arg))
            {
                server_response =errorMsg;
                return Status::CANCELLED;
            }
             server_response = check_registration_status(stoi(arg));

        }
        else if (cmd == "vote_count")
        {

            server_response = vote_count(arg);

        }
        else
        {
            server_response =errorMsg;

        }
    }
    // 2 argument
    else if (inputs.size() == 3)
    {
        string cmd = inputs[0];
        string arg1 = inputs[1];
        string arg2 = inputs[2];

        if (cmd == "add_candidate")
        {

            server_response = add_candidate(arg1, arg2);

        }
        else if (cmd == "vote_for")
        {
            if (!isNumber(arg2))
            {

                server_response = errorMsg;

                 return Status::CANCELLED;
            }

            server_response = vote_for(arg1, stoi(arg2));

        }
        else if (cmd == "check_voter_status")
        {
            if (!isNumber(arg1) || !isNumber(arg2))
            {

                server_response =errorMsg;

                return Status::CANCELLED;
            }

            server_response = check_voter_status(stoi(arg1), stoi(arg2));

        }
        else
        {

           server_response = errorMsg;

        }
    }
    else
    {

        server_response = errorMsg;

    }


  response->set_feedback(server_response);
  return Status::OK;
   
}
  

int main(int argc, char *argv[]){
  
    string port = "10000";

    // 1. parse command
    // -r -p -a
    int option;
    while ((option = getopt(argc, argv, "a:rp:")) != -1)
    {
        switch (option)
        {
        case 'a':
            password = optarg;
            changePassword = true;
            break;
        case 'r':
            // read file
            recover();
            break;
        case 'p':
            //connect to port
            if (!isValidPortNumber(optarg))
            {
                cout << "ERROR: Invalid port number" << endl;
                exit(0);
                break;
            }
            port = stoi(optarg);
            break;
        case '?':
            exit(0);
            break;
        }
    }

  
  string server_address("0.0.0.0:" + port);
  VoteServer service;

  ServerBuilder builder;
  // Listen on the given address without any authentication mechanism.
  builder.AddListeningPort(server_address, InsecureServerCredentials());
  // Register "service" as the instance through which we'll communicate with
  // clients. In this case it corresponds to an *synchronous* service.
  builder.RegisterService(&service);
  
//   grpc::ResourceQuota rq;
//   rq.SetMaxThreads(9999);
//   builder.SetResourceQuota(rq);

  // Finally assemble the server.
  unique_ptr<Server> server(builder.BuildAndStart());
  cout << "Server listening on " << server_address << endl;

  // Wait for the server to shutdown. Note that some other thread must be
  // responsible for shutting down the server for this call to ever return.
  server->Wait();
  return 0;
}