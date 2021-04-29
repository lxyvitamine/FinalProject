#include "finalec2.grpc.pb.h"
#include<string.h>
#include "helper.h"


using grpc::ServerContext;
using grpc::Status;
using protos::Request;
using protos::VoteManager;
using protos::Response;
using protos::Empty;

class VoteServer final : public VoteManager::Service {
 
 public:
 ~VoteServer(){};
 Status parseUserCmd(ServerContext* context, const Request* request, Response* response);
 Status shuttheserver(ServerContext* context, const Empty* request, Response* response);
  
	
};