// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: finalec2.proto
#ifndef GRPC_finalec2_2eproto__INCLUDED
#define GRPC_finalec2_2eproto__INCLUDED

#include "finalec2.pb.h"

#include <functional>
#include <grpc/impl/codegen/port_platform.h>
#include <grpcpp/impl/codegen/async_generic_service.h>
#include <grpcpp/impl/codegen/async_stream.h>
#include <grpcpp/impl/codegen/async_unary_call.h>
#include <grpcpp/impl/codegen/client_callback.h>
#include <grpcpp/impl/codegen/client_context.h>
#include <grpcpp/impl/codegen/completion_queue.h>
#include <grpcpp/impl/codegen/message_allocator.h>
#include <grpcpp/impl/codegen/method_handler.h>
#include <grpcpp/impl/codegen/proto_utils.h>
#include <grpcpp/impl/codegen/rpc_method.h>
#include <grpcpp/impl/codegen/server_callback.h>
#include <grpcpp/impl/codegen/server_callback_handlers.h>
#include <grpcpp/impl/codegen/server_context.h>
#include <grpcpp/impl/codegen/service_type.h>
#include <grpcpp/impl/codegen/status.h>
#include <grpcpp/impl/codegen/stub_options.h>
#include <grpcpp/impl/codegen/sync_stream.h>

namespace protos {

class VoteManager final {
 public:
  static constexpr char const* service_full_name() {
    return "protos.VoteManager";
  }
  class StubInterface {
   public:
    virtual ~StubInterface() {}
    virtual ::grpc::Status parseUserCmd(::grpc::ClientContext* context, const ::protos::Request& request, ::protos::Response* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::protos::Response>> AsyncparseUserCmd(::grpc::ClientContext* context, const ::protos::Request& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::protos::Response>>(AsyncparseUserCmdRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::protos::Response>> PrepareAsyncparseUserCmd(::grpc::ClientContext* context, const ::protos::Request& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::protos::Response>>(PrepareAsyncparseUserCmdRaw(context, request, cq));
    }
    class experimental_async_interface {
     public:
      virtual ~experimental_async_interface() {}
      virtual void parseUserCmd(::grpc::ClientContext* context, const ::protos::Request* request, ::protos::Response* response, std::function<void(::grpc::Status)>) = 0;
      #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      virtual void parseUserCmd(::grpc::ClientContext* context, const ::protos::Request* request, ::protos::Response* response, ::grpc::ClientUnaryReactor* reactor) = 0;
      #else
      virtual void parseUserCmd(::grpc::ClientContext* context, const ::protos::Request* request, ::protos::Response* response, ::grpc::experimental::ClientUnaryReactor* reactor) = 0;
      #endif
    };
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
    typedef class experimental_async_interface async_interface;
    #endif
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
    async_interface* async() { return experimental_async(); }
    #endif
    virtual class experimental_async_interface* experimental_async() { return nullptr; }
  private:
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::protos::Response>* AsyncparseUserCmdRaw(::grpc::ClientContext* context, const ::protos::Request& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::protos::Response>* PrepareAsyncparseUserCmdRaw(::grpc::ClientContext* context, const ::protos::Request& request, ::grpc::CompletionQueue* cq) = 0;
  };
  class Stub final : public StubInterface {
   public:
    Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel);
    ::grpc::Status parseUserCmd(::grpc::ClientContext* context, const ::protos::Request& request, ::protos::Response* response) override;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::protos::Response>> AsyncparseUserCmd(::grpc::ClientContext* context, const ::protos::Request& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::protos::Response>>(AsyncparseUserCmdRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::protos::Response>> PrepareAsyncparseUserCmd(::grpc::ClientContext* context, const ::protos::Request& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::protos::Response>>(PrepareAsyncparseUserCmdRaw(context, request, cq));
    }
    class experimental_async final :
      public StubInterface::experimental_async_interface {
     public:
      void parseUserCmd(::grpc::ClientContext* context, const ::protos::Request* request, ::protos::Response* response, std::function<void(::grpc::Status)>) override;
      #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      void parseUserCmd(::grpc::ClientContext* context, const ::protos::Request* request, ::protos::Response* response, ::grpc::ClientUnaryReactor* reactor) override;
      #else
      void parseUserCmd(::grpc::ClientContext* context, const ::protos::Request* request, ::protos::Response* response, ::grpc::experimental::ClientUnaryReactor* reactor) override;
      #endif
     private:
      friend class Stub;
      explicit experimental_async(Stub* stub): stub_(stub) { }
      Stub* stub() { return stub_; }
      Stub* stub_;
    };
    class experimental_async_interface* experimental_async() override { return &async_stub_; }

   private:
    std::shared_ptr< ::grpc::ChannelInterface> channel_;
    class experimental_async async_stub_{this};
    ::grpc::ClientAsyncResponseReader< ::protos::Response>* AsyncparseUserCmdRaw(::grpc::ClientContext* context, const ::protos::Request& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::protos::Response>* PrepareAsyncparseUserCmdRaw(::grpc::ClientContext* context, const ::protos::Request& request, ::grpc::CompletionQueue* cq) override;
    const ::grpc::internal::RpcMethod rpcmethod_parseUserCmd_;
  };
  static std::unique_ptr<Stub> NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options = ::grpc::StubOptions());

  class Service : public ::grpc::Service {
   public:
    Service();
    virtual ~Service();
    virtual ::grpc::Status parseUserCmd(::grpc::ServerContext* context, const ::protos::Request* request, ::protos::Response* response);
  };
  template <class BaseClass>
  class WithAsyncMethod_parseUserCmd : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithAsyncMethod_parseUserCmd() {
      ::grpc::Service::MarkMethodAsync(0);
    }
    ~WithAsyncMethod_parseUserCmd() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status parseUserCmd(::grpc::ServerContext* /*context*/, const ::protos::Request* /*request*/, ::protos::Response* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestparseUserCmd(::grpc::ServerContext* context, ::protos::Request* request, ::grpc::ServerAsyncResponseWriter< ::protos::Response>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(0, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  typedef WithAsyncMethod_parseUserCmd<Service > AsyncService;
  template <class BaseClass>
  class ExperimentalWithCallbackMethod_parseUserCmd : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    ExperimentalWithCallbackMethod_parseUserCmd() {
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      ::grpc::Service::
    #else
      ::grpc::Service::experimental().
    #endif
        MarkMethodCallback(0,
          new ::grpc::internal::CallbackUnaryHandler< ::protos::Request, ::protos::Response>(
            [this](
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
                   ::grpc::CallbackServerContext*
    #else
                   ::grpc::experimental::CallbackServerContext*
    #endif
                     context, const ::protos::Request* request, ::protos::Response* response) { return this->parseUserCmd(context, request, response); }));}
    void SetMessageAllocatorFor_parseUserCmd(
        ::grpc::experimental::MessageAllocator< ::protos::Request, ::protos::Response>* allocator) {
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      ::grpc::internal::MethodHandler* const handler = ::grpc::Service::GetHandler(0);
    #else
      ::grpc::internal::MethodHandler* const handler = ::grpc::Service::experimental().GetHandler(0);
    #endif
      static_cast<::grpc::internal::CallbackUnaryHandler< ::protos::Request, ::protos::Response>*>(handler)
              ->SetMessageAllocator(allocator);
    }
    ~ExperimentalWithCallbackMethod_parseUserCmd() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status parseUserCmd(::grpc::ServerContext* /*context*/, const ::protos::Request* /*request*/, ::protos::Response* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
    virtual ::grpc::ServerUnaryReactor* parseUserCmd(
      ::grpc::CallbackServerContext* /*context*/, const ::protos::Request* /*request*/, ::protos::Response* /*response*/)
    #else
    virtual ::grpc::experimental::ServerUnaryReactor* parseUserCmd(
      ::grpc::experimental::CallbackServerContext* /*context*/, const ::protos::Request* /*request*/, ::protos::Response* /*response*/)
    #endif
      { return nullptr; }
  };
  #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
  typedef ExperimentalWithCallbackMethod_parseUserCmd<Service > CallbackService;
  #endif

  typedef ExperimentalWithCallbackMethod_parseUserCmd<Service > ExperimentalCallbackService;
  template <class BaseClass>
  class WithGenericMethod_parseUserCmd : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithGenericMethod_parseUserCmd() {
      ::grpc::Service::MarkMethodGeneric(0);
    }
    ~WithGenericMethod_parseUserCmd() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status parseUserCmd(::grpc::ServerContext* /*context*/, const ::protos::Request* /*request*/, ::protos::Response* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithRawMethod_parseUserCmd : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawMethod_parseUserCmd() {
      ::grpc::Service::MarkMethodRaw(0);
    }
    ~WithRawMethod_parseUserCmd() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status parseUserCmd(::grpc::ServerContext* /*context*/, const ::protos::Request* /*request*/, ::protos::Response* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestparseUserCmd(::grpc::ServerContext* context, ::grpc::ByteBuffer* request, ::grpc::ServerAsyncResponseWriter< ::grpc::ByteBuffer>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(0, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class ExperimentalWithRawCallbackMethod_parseUserCmd : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    ExperimentalWithRawCallbackMethod_parseUserCmd() {
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      ::grpc::Service::
    #else
      ::grpc::Service::experimental().
    #endif
        MarkMethodRawCallback(0,
          new ::grpc::internal::CallbackUnaryHandler< ::grpc::ByteBuffer, ::grpc::ByteBuffer>(
            [this](
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
                   ::grpc::CallbackServerContext*
    #else
                   ::grpc::experimental::CallbackServerContext*
    #endif
                     context, const ::grpc::ByteBuffer* request, ::grpc::ByteBuffer* response) { return this->parseUserCmd(context, request, response); }));
    }
    ~ExperimentalWithRawCallbackMethod_parseUserCmd() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status parseUserCmd(::grpc::ServerContext* /*context*/, const ::protos::Request* /*request*/, ::protos::Response* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
    virtual ::grpc::ServerUnaryReactor* parseUserCmd(
      ::grpc::CallbackServerContext* /*context*/, const ::grpc::ByteBuffer* /*request*/, ::grpc::ByteBuffer* /*response*/)
    #else
    virtual ::grpc::experimental::ServerUnaryReactor* parseUserCmd(
      ::grpc::experimental::CallbackServerContext* /*context*/, const ::grpc::ByteBuffer* /*request*/, ::grpc::ByteBuffer* /*response*/)
    #endif
      { return nullptr; }
  };
  template <class BaseClass>
  class WithStreamedUnaryMethod_parseUserCmd : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithStreamedUnaryMethod_parseUserCmd() {
      ::grpc::Service::MarkMethodStreamed(0,
        new ::grpc::internal::StreamedUnaryHandler<
          ::protos::Request, ::protos::Response>(
            [this](::grpc::ServerContext* context,
                   ::grpc::ServerUnaryStreamer<
                     ::protos::Request, ::protos::Response>* streamer) {
                       return this->StreamedparseUserCmd(context,
                         streamer);
                  }));
    }
    ~WithStreamedUnaryMethod_parseUserCmd() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable regular version of this method
    ::grpc::Status parseUserCmd(::grpc::ServerContext* /*context*/, const ::protos::Request* /*request*/, ::protos::Response* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    // replace default version of method with streamed unary
    virtual ::grpc::Status StreamedparseUserCmd(::grpc::ServerContext* context, ::grpc::ServerUnaryStreamer< ::protos::Request,::protos::Response>* server_unary_streamer) = 0;
  };
  typedef WithStreamedUnaryMethod_parseUserCmd<Service > StreamedUnaryService;
  typedef Service SplitStreamedService;
  typedef WithStreamedUnaryMethod_parseUserCmd<Service > StreamedService;
};

}  // namespace protos


#endif  // GRPC_finalec2_2eproto__INCLUDED
