#pragma once

#include <grpcpp/grpcpp.h>
#include <memory>
#include <string>




#include <iostream>
#include <memory>
#include <string>

#include "hsm_grpc_server.hpp"

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

#include "hsm.grpc.pb.h"

#include "HSM.h"

using grpc::ServerContext;
using grpc::Status;
using HSM_gRpc::DecryptReply;
using HSM_gRpc::DecryptRequest;
using HSM_gRpc::EncryptReply;
using HSM_gRpc::EncryptRequest;
using HSM_gRpc::GenKeyReply;
using HSM_gRpc::GenKeyRequest;
using HSM_gRpc::HSM_RPC;
using HSM_gRpc::SignReply;
using HSM_gRpc::SignRequest;
using HSM_gRpc::VerifyReply;
using HSM_gRpc::VerifyRequest;



namespace HSMns {
namespace RPC {


class HSM_RPCServiceImpl final : public HSM_RPC::Service
{

  Status Encrypt(ServerContext *context,
                 const EncryptRequest *request, EncryptReply *reply) override;

  Status Decrypt(ServerContext *context,
                 const DecryptRequest *request, DecryptReply *reply) override;

  Status Sign(ServerContext *context,
              const SignRequest *request, SignReply *reply) override;

  Status Verify(ServerContext *context,
                const VerifyRequest *request, VerifyReply *reply) override;

  Status GenKey(ServerContext *context,
                const GenKeyRequest *request, GenKeyReply *reply) override;

};


class HSMServer {
    HSM_RPCServiceImpl service;
    std::unique_ptr<grpc::Server> server;

public:
    explicit HSMServer(std::string const &server_address = "0.0.0.0:50051", std::shared_ptr<grpc::ServerCredentials> credentials = grpc::InsecureServerCredentials());
    void Wait();
    void shutdown();
};


} // namespace HSMns 
} // namespace RPC 
