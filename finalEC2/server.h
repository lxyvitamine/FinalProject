#include "finalec2.grpc.pb.h"
#include<string.h>
#include "helper.h"


using grpc::ServerContext;
using grpc::Status;
using protos::Request;
using protos::VoteManager;
using protos::Response;

class VoteServer final : public VoteManager::Service {
  
 public:
 Status parseUserCmd(ServerContext* context, const Request* request, Response* response);
	
};