#include <grpcpp/grpcpp.h>
#include <memory>
#include "finalec2.grpc.pb.h"

using grpc::Channel;
using protos::VoteManager;
//using protos::BookmarkStatus;

class VoteClient {
 private:
  std::unique_ptr<VoteManager::Stub> stub;

 public:
  VoteClient(std::shared_ptr<Channel> channel)
      : stub(VoteManager::NewStub(channel)) {}

  void parseUserCmd(const std::string& input);
};
