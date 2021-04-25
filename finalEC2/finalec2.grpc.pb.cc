// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: finalec2.proto

#include "finalec2.pb.h"
#include "finalec2.grpc.pb.h"

#include <functional>
#include <grpcpp/impl/codegen/async_stream.h>
#include <grpcpp/impl/codegen/async_unary_call.h>
#include <grpcpp/impl/codegen/channel_interface.h>
#include <grpcpp/impl/codegen/client_unary_call.h>
#include <grpcpp/impl/codegen/client_callback.h>
#include <grpcpp/impl/codegen/message_allocator.h>
#include <grpcpp/impl/codegen/method_handler.h>
#include <grpcpp/impl/codegen/rpc_service_method.h>
#include <grpcpp/impl/codegen/server_callback.h>
#include <grpcpp/impl/codegen/server_callback_handlers.h>
#include <grpcpp/impl/codegen/server_context.h>
#include <grpcpp/impl/codegen/service_type.h>
#include <grpcpp/impl/codegen/sync_stream.h>
namespace protos {

static const char* VoteManager_method_names[] = {
  "/protos.VoteManager/parseUserCmd",
};

std::unique_ptr< VoteManager::Stub> VoteManager::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< VoteManager::Stub> stub(new VoteManager::Stub(channel));
  return stub;
}

VoteManager::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel)
  : channel_(channel), rpcmethod_parseUserCmd_(VoteManager_method_names[0], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status VoteManager::Stub::parseUserCmd(::grpc::ClientContext* context, const ::protos::Request& request, ::protos::Response* response) {
  return ::grpc::internal::BlockingUnaryCall< ::protos::Request, ::protos::Response, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_parseUserCmd_, context, request, response);
}

void VoteManager::Stub::experimental_async::parseUserCmd(::grpc::ClientContext* context, const ::protos::Request* request, ::protos::Response* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::protos::Request, ::protos::Response, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_parseUserCmd_, context, request, response, std::move(f));
}

void VoteManager::Stub::experimental_async::parseUserCmd(::grpc::ClientContext* context, const ::protos::Request* request, ::protos::Response* response, ::grpc::experimental::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_parseUserCmd_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::protos::Response>* VoteManager::Stub::PrepareAsyncparseUserCmdRaw(::grpc::ClientContext* context, const ::protos::Request& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::protos::Response, ::protos::Request, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_parseUserCmd_, context, request);
}

::grpc::ClientAsyncResponseReader< ::protos::Response>* VoteManager::Stub::AsyncparseUserCmdRaw(::grpc::ClientContext* context, const ::protos::Request& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncparseUserCmdRaw(context, request, cq);
  result->StartCall();
  return result;
}

VoteManager::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      VoteManager_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< VoteManager::Service, ::protos::Request, ::protos::Response, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](VoteManager::Service* service,
             ::grpc::ServerContext* ctx,
             const ::protos::Request* req,
             ::protos::Response* resp) {
               return service->parseUserCmd(ctx, req, resp);
             }, this)));
}

VoteManager::Service::~Service() {
}

::grpc::Status VoteManager::Service::parseUserCmd(::grpc::ServerContext* context, const ::protos::Request* request, ::protos::Response* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace protos

