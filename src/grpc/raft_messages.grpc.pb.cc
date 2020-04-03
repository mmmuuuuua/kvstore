// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: raft_messages.proto

#include "raft_messages.pb.h"
#include "raft_messages.grpc.pb.h"

#include <functional>
#include <grpcpp/impl/codegen/async_stream.h>
#include <grpcpp/impl/codegen/async_unary_call.h>
#include <grpcpp/impl/codegen/channel_interface.h>
#include <grpcpp/impl/codegen/client_unary_call.h>
#include <grpcpp/impl/codegen/client_callback.h>
#include <grpcpp/impl/codegen/method_handler_impl.h>
#include <grpcpp/impl/codegen/rpc_service_method.h>
#include <grpcpp/impl/codegen/server_callback.h>
#include <grpcpp/impl/codegen/service_type.h>
#include <grpcpp/impl/codegen/sync_stream.h>
namespace raft_messages {

static const char* RaftMessages_method_names[] = {
  "/raft_messages.RaftMessages/RequestVote",
  "/raft_messages.RaftMessages/AppendEntries",
  "/raft_messages.RaftMessages/InstallSnapshot",
};

std::unique_ptr< RaftMessages::Stub> RaftMessages::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< RaftMessages::Stub> stub(new RaftMessages::Stub(channel));
  return stub;
}

RaftMessages::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel)
  : channel_(channel), rpcmethod_RequestVote_(RaftMessages_method_names[0], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_AppendEntries_(RaftMessages_method_names[1], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_InstallSnapshot_(RaftMessages_method_names[2], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status RaftMessages::Stub::RequestVote(::grpc::ClientContext* context, const ::raft_messages::RequestVoteRequest& request, ::raft_messages::RequestVoteResponse* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_RequestVote_, context, request, response);
}

void RaftMessages::Stub::experimental_async::RequestVote(::grpc::ClientContext* context, const ::raft_messages::RequestVoteRequest* request, ::raft_messages::RequestVoteResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc_impl::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_RequestVote_, context, request, response, std::move(f));
}

void RaftMessages::Stub::experimental_async::RequestVote(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::raft_messages::RequestVoteResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc_impl::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_RequestVote_, context, request, response, std::move(f));
}

void RaftMessages::Stub::experimental_async::RequestVote(::grpc::ClientContext* context, const ::raft_messages::RequestVoteRequest* request, ::raft_messages::RequestVoteResponse* response, ::grpc::experimental::ClientUnaryReactor* reactor) {
  ::grpc_impl::internal::ClientCallbackUnaryFactory::Create(stub_->channel_.get(), stub_->rpcmethod_RequestVote_, context, request, response, reactor);
}

void RaftMessages::Stub::experimental_async::RequestVote(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::raft_messages::RequestVoteResponse* response, ::grpc::experimental::ClientUnaryReactor* reactor) {
  ::grpc_impl::internal::ClientCallbackUnaryFactory::Create(stub_->channel_.get(), stub_->rpcmethod_RequestVote_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::raft_messages::RequestVoteResponse>* RaftMessages::Stub::AsyncRequestVoteRaw(::grpc::ClientContext* context, const ::raft_messages::RequestVoteRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncResponseReaderFactory< ::raft_messages::RequestVoteResponse>::Create(channel_.get(), cq, rpcmethod_RequestVote_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::raft_messages::RequestVoteResponse>* RaftMessages::Stub::PrepareAsyncRequestVoteRaw(::grpc::ClientContext* context, const ::raft_messages::RequestVoteRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncResponseReaderFactory< ::raft_messages::RequestVoteResponse>::Create(channel_.get(), cq, rpcmethod_RequestVote_, context, request, false);
}

::grpc::Status RaftMessages::Stub::AppendEntries(::grpc::ClientContext* context, const ::raft_messages::AppendEntriesRequest& request, ::raft_messages::AppendEntriesResponse* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_AppendEntries_, context, request, response);
}

void RaftMessages::Stub::experimental_async::AppendEntries(::grpc::ClientContext* context, const ::raft_messages::AppendEntriesRequest* request, ::raft_messages::AppendEntriesResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc_impl::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_AppendEntries_, context, request, response, std::move(f));
}

void RaftMessages::Stub::experimental_async::AppendEntries(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::raft_messages::AppendEntriesResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc_impl::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_AppendEntries_, context, request, response, std::move(f));
}

void RaftMessages::Stub::experimental_async::AppendEntries(::grpc::ClientContext* context, const ::raft_messages::AppendEntriesRequest* request, ::raft_messages::AppendEntriesResponse* response, ::grpc::experimental::ClientUnaryReactor* reactor) {
  ::grpc_impl::internal::ClientCallbackUnaryFactory::Create(stub_->channel_.get(), stub_->rpcmethod_AppendEntries_, context, request, response, reactor);
}

void RaftMessages::Stub::experimental_async::AppendEntries(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::raft_messages::AppendEntriesResponse* response, ::grpc::experimental::ClientUnaryReactor* reactor) {
  ::grpc_impl::internal::ClientCallbackUnaryFactory::Create(stub_->channel_.get(), stub_->rpcmethod_AppendEntries_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::raft_messages::AppendEntriesResponse>* RaftMessages::Stub::AsyncAppendEntriesRaw(::grpc::ClientContext* context, const ::raft_messages::AppendEntriesRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncResponseReaderFactory< ::raft_messages::AppendEntriesResponse>::Create(channel_.get(), cq, rpcmethod_AppendEntries_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::raft_messages::AppendEntriesResponse>* RaftMessages::Stub::PrepareAsyncAppendEntriesRaw(::grpc::ClientContext* context, const ::raft_messages::AppendEntriesRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncResponseReaderFactory< ::raft_messages::AppendEntriesResponse>::Create(channel_.get(), cq, rpcmethod_AppendEntries_, context, request, false);
}

::grpc::Status RaftMessages::Stub::InstallSnapshot(::grpc::ClientContext* context, const ::raft_messages::InstallSnapshotRequest& request, ::raft_messages::InstallSnapshotResponse* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_InstallSnapshot_, context, request, response);
}

void RaftMessages::Stub::experimental_async::InstallSnapshot(::grpc::ClientContext* context, const ::raft_messages::InstallSnapshotRequest* request, ::raft_messages::InstallSnapshotResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc_impl::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_InstallSnapshot_, context, request, response, std::move(f));
}

void RaftMessages::Stub::experimental_async::InstallSnapshot(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::raft_messages::InstallSnapshotResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc_impl::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_InstallSnapshot_, context, request, response, std::move(f));
}

void RaftMessages::Stub::experimental_async::InstallSnapshot(::grpc::ClientContext* context, const ::raft_messages::InstallSnapshotRequest* request, ::raft_messages::InstallSnapshotResponse* response, ::grpc::experimental::ClientUnaryReactor* reactor) {
  ::grpc_impl::internal::ClientCallbackUnaryFactory::Create(stub_->channel_.get(), stub_->rpcmethod_InstallSnapshot_, context, request, response, reactor);
}

void RaftMessages::Stub::experimental_async::InstallSnapshot(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::raft_messages::InstallSnapshotResponse* response, ::grpc::experimental::ClientUnaryReactor* reactor) {
  ::grpc_impl::internal::ClientCallbackUnaryFactory::Create(stub_->channel_.get(), stub_->rpcmethod_InstallSnapshot_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::raft_messages::InstallSnapshotResponse>* RaftMessages::Stub::AsyncInstallSnapshotRaw(::grpc::ClientContext* context, const ::raft_messages::InstallSnapshotRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncResponseReaderFactory< ::raft_messages::InstallSnapshotResponse>::Create(channel_.get(), cq, rpcmethod_InstallSnapshot_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::raft_messages::InstallSnapshotResponse>* RaftMessages::Stub::PrepareAsyncInstallSnapshotRaw(::grpc::ClientContext* context, const ::raft_messages::InstallSnapshotRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncResponseReaderFactory< ::raft_messages::InstallSnapshotResponse>::Create(channel_.get(), cq, rpcmethod_InstallSnapshot_, context, request, false);
}

RaftMessages::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      RaftMessages_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< RaftMessages::Service, ::raft_messages::RequestVoteRequest, ::raft_messages::RequestVoteResponse>(
          std::mem_fn(&RaftMessages::Service::RequestVote), this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      RaftMessages_method_names[1],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< RaftMessages::Service, ::raft_messages::AppendEntriesRequest, ::raft_messages::AppendEntriesResponse>(
          std::mem_fn(&RaftMessages::Service::AppendEntries), this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      RaftMessages_method_names[2],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< RaftMessages::Service, ::raft_messages::InstallSnapshotRequest, ::raft_messages::InstallSnapshotResponse>(
          std::mem_fn(&RaftMessages::Service::InstallSnapshot), this)));
}

RaftMessages::Service::~Service() {
}

::grpc::Status RaftMessages::Service::RequestVote(::grpc::ServerContext* context, const ::raft_messages::RequestVoteRequest* request, ::raft_messages::RequestVoteResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status RaftMessages::Service::AppendEntries(::grpc::ServerContext* context, const ::raft_messages::AppendEntriesRequest* request, ::raft_messages::AppendEntriesResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status RaftMessages::Service::InstallSnapshot(::grpc::ServerContext* context, const ::raft_messages::InstallSnapshotRequest* request, ::raft_messages::InstallSnapshotResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


static const char* RaftStreamMessages_method_names[] = {
  "/raft_messages.RaftStreamMessages/RequestVote",
  "/raft_messages.RaftStreamMessages/AppendEntries",
  "/raft_messages.RaftStreamMessages/InstallSnapshot",
};

std::unique_ptr< RaftStreamMessages::Stub> RaftStreamMessages::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< RaftStreamMessages::Stub> stub(new RaftStreamMessages::Stub(channel));
  return stub;
}

RaftStreamMessages::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel)
  : channel_(channel), rpcmethod_RequestVote_(RaftStreamMessages_method_names[0], ::grpc::internal::RpcMethod::BIDI_STREAMING, channel)
  , rpcmethod_AppendEntries_(RaftStreamMessages_method_names[1], ::grpc::internal::RpcMethod::BIDI_STREAMING, channel)
  , rpcmethod_InstallSnapshot_(RaftStreamMessages_method_names[2], ::grpc::internal::RpcMethod::BIDI_STREAMING, channel)
  {}

::grpc::ClientReaderWriter< ::raft_messages::RequestVoteRequest, ::raft_messages::RequestVoteResponse>* RaftStreamMessages::Stub::RequestVoteRaw(::grpc::ClientContext* context) {
  return ::grpc_impl::internal::ClientReaderWriterFactory< ::raft_messages::RequestVoteRequest, ::raft_messages::RequestVoteResponse>::Create(channel_.get(), rpcmethod_RequestVote_, context);
}

void RaftStreamMessages::Stub::experimental_async::RequestVote(::grpc::ClientContext* context, ::grpc::experimental::ClientBidiReactor< ::raft_messages::RequestVoteRequest,::raft_messages::RequestVoteResponse>* reactor) {
  ::grpc_impl::internal::ClientCallbackReaderWriterFactory< ::raft_messages::RequestVoteRequest,::raft_messages::RequestVoteResponse>::Create(stub_->channel_.get(), stub_->rpcmethod_RequestVote_, context, reactor);
}

::grpc::ClientAsyncReaderWriter< ::raft_messages::RequestVoteRequest, ::raft_messages::RequestVoteResponse>* RaftStreamMessages::Stub::AsyncRequestVoteRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq, void* tag) {
  return ::grpc_impl::internal::ClientAsyncReaderWriterFactory< ::raft_messages::RequestVoteRequest, ::raft_messages::RequestVoteResponse>::Create(channel_.get(), cq, rpcmethod_RequestVote_, context, true, tag);
}

::grpc::ClientAsyncReaderWriter< ::raft_messages::RequestVoteRequest, ::raft_messages::RequestVoteResponse>* RaftStreamMessages::Stub::PrepareAsyncRequestVoteRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncReaderWriterFactory< ::raft_messages::RequestVoteRequest, ::raft_messages::RequestVoteResponse>::Create(channel_.get(), cq, rpcmethod_RequestVote_, context, false, nullptr);
}

::grpc::ClientReaderWriter< ::raft_messages::AppendEntriesRequest, ::raft_messages::AppendEntriesResponse>* RaftStreamMessages::Stub::AppendEntriesRaw(::grpc::ClientContext* context) {
  return ::grpc_impl::internal::ClientReaderWriterFactory< ::raft_messages::AppendEntriesRequest, ::raft_messages::AppendEntriesResponse>::Create(channel_.get(), rpcmethod_AppendEntries_, context);
}

void RaftStreamMessages::Stub::experimental_async::AppendEntries(::grpc::ClientContext* context, ::grpc::experimental::ClientBidiReactor< ::raft_messages::AppendEntriesRequest,::raft_messages::AppendEntriesResponse>* reactor) {
  ::grpc_impl::internal::ClientCallbackReaderWriterFactory< ::raft_messages::AppendEntriesRequest,::raft_messages::AppendEntriesResponse>::Create(stub_->channel_.get(), stub_->rpcmethod_AppendEntries_, context, reactor);
}

::grpc::ClientAsyncReaderWriter< ::raft_messages::AppendEntriesRequest, ::raft_messages::AppendEntriesResponse>* RaftStreamMessages::Stub::AsyncAppendEntriesRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq, void* tag) {
  return ::grpc_impl::internal::ClientAsyncReaderWriterFactory< ::raft_messages::AppendEntriesRequest, ::raft_messages::AppendEntriesResponse>::Create(channel_.get(), cq, rpcmethod_AppendEntries_, context, true, tag);
}

::grpc::ClientAsyncReaderWriter< ::raft_messages::AppendEntriesRequest, ::raft_messages::AppendEntriesResponse>* RaftStreamMessages::Stub::PrepareAsyncAppendEntriesRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncReaderWriterFactory< ::raft_messages::AppendEntriesRequest, ::raft_messages::AppendEntriesResponse>::Create(channel_.get(), cq, rpcmethod_AppendEntries_, context, false, nullptr);
}

::grpc::ClientReaderWriter< ::raft_messages::InstallSnapshotRequest, ::raft_messages::InstallSnapshotResponse>* RaftStreamMessages::Stub::InstallSnapshotRaw(::grpc::ClientContext* context) {
  return ::grpc_impl::internal::ClientReaderWriterFactory< ::raft_messages::InstallSnapshotRequest, ::raft_messages::InstallSnapshotResponse>::Create(channel_.get(), rpcmethod_InstallSnapshot_, context);
}

void RaftStreamMessages::Stub::experimental_async::InstallSnapshot(::grpc::ClientContext* context, ::grpc::experimental::ClientBidiReactor< ::raft_messages::InstallSnapshotRequest,::raft_messages::InstallSnapshotResponse>* reactor) {
  ::grpc_impl::internal::ClientCallbackReaderWriterFactory< ::raft_messages::InstallSnapshotRequest,::raft_messages::InstallSnapshotResponse>::Create(stub_->channel_.get(), stub_->rpcmethod_InstallSnapshot_, context, reactor);
}

::grpc::ClientAsyncReaderWriter< ::raft_messages::InstallSnapshotRequest, ::raft_messages::InstallSnapshotResponse>* RaftStreamMessages::Stub::AsyncInstallSnapshotRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq, void* tag) {
  return ::grpc_impl::internal::ClientAsyncReaderWriterFactory< ::raft_messages::InstallSnapshotRequest, ::raft_messages::InstallSnapshotResponse>::Create(channel_.get(), cq, rpcmethod_InstallSnapshot_, context, true, tag);
}

::grpc::ClientAsyncReaderWriter< ::raft_messages::InstallSnapshotRequest, ::raft_messages::InstallSnapshotResponse>* RaftStreamMessages::Stub::PrepareAsyncInstallSnapshotRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq) {
  return ::grpc_impl::internal::ClientAsyncReaderWriterFactory< ::raft_messages::InstallSnapshotRequest, ::raft_messages::InstallSnapshotResponse>::Create(channel_.get(), cq, rpcmethod_InstallSnapshot_, context, false, nullptr);
}

RaftStreamMessages::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      RaftStreamMessages_method_names[0],
      ::grpc::internal::RpcMethod::BIDI_STREAMING,
      new ::grpc::internal::BidiStreamingHandler< RaftStreamMessages::Service, ::raft_messages::RequestVoteRequest, ::raft_messages::RequestVoteResponse>(
          std::mem_fn(&RaftStreamMessages::Service::RequestVote), this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      RaftStreamMessages_method_names[1],
      ::grpc::internal::RpcMethod::BIDI_STREAMING,
      new ::grpc::internal::BidiStreamingHandler< RaftStreamMessages::Service, ::raft_messages::AppendEntriesRequest, ::raft_messages::AppendEntriesResponse>(
          std::mem_fn(&RaftStreamMessages::Service::AppendEntries), this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      RaftStreamMessages_method_names[2],
      ::grpc::internal::RpcMethod::BIDI_STREAMING,
      new ::grpc::internal::BidiStreamingHandler< RaftStreamMessages::Service, ::raft_messages::InstallSnapshotRequest, ::raft_messages::InstallSnapshotResponse>(
          std::mem_fn(&RaftStreamMessages::Service::InstallSnapshot), this)));
}

RaftStreamMessages::Service::~Service() {
}

::grpc::Status RaftStreamMessages::Service::RequestVote(::grpc::ServerContext* context, ::grpc::ServerReaderWriter< ::raft_messages::RequestVoteResponse, ::raft_messages::RequestVoteRequest>* stream) {
  (void) context;
  (void) stream;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status RaftStreamMessages::Service::AppendEntries(::grpc::ServerContext* context, ::grpc::ServerReaderWriter< ::raft_messages::AppendEntriesResponse, ::raft_messages::AppendEntriesRequest>* stream) {
  (void) context;
  (void) stream;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status RaftStreamMessages::Service::InstallSnapshot(::grpc::ServerContext* context, ::grpc::ServerReaderWriter< ::raft_messages::InstallSnapshotResponse, ::raft_messages::InstallSnapshotRequest>* stream) {
  (void) context;
  (void) stream;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace raft_messages

