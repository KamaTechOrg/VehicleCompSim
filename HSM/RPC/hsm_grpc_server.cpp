/*
 *
 * Copyright 2015 gRPC authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <iostream>
#include <memory>
#include <string>

#include "hsm_grpc_server.hpp"

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

#include "hsm.grpc.pb.h"

#include "HSM.h"

using grpc::Server;
using grpc::ServerBuilder;
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


grpc::Status HSM_RPCServiceImpl::Encrypt(ServerContext *context,
                                    const EncryptRequest *request, EncryptReply *reply) 
{
    std::vector<uint8_t> clr_data(request->clr_data().begin(), request->clr_data().end());
    std::vector<uint8_t> enc_data;
    auto type = static_cast<HSMns::ENCRYPTION_ALGORITHM_TYPE>(request->algo_type());
    HSMns::Ident id(request->my_id().id());
    uint32_t key_id = request->key_id();
    bool needPrivilege = request->need_privilege();
    auto &hsm = HSMns::getInstance();
    HSMns::HSM_STATUS status = hsm.encrypt(
        clr_data,
        enc_data,
        type,
        id,
        key_id,
        needPrivilege);
    if (status == HSMns::HSM_STATUS::HSM_Good)
    {
        reply->set_enc_data(enc_data.data(), enc_data.size());
    }
    reply->set_status(static_cast<HSM_gRpc::HSM_STATUS>(status));
    return Status::OK;
}

grpc::Status HSM_RPCServiceImpl::Decrypt(ServerContext *context,
                                    const DecryptRequest *request, DecryptReply *reply) 
{
    std::vector<uint8_t> enc_data(request->enc_data().begin(), request->enc_data().end());
    std::vector<uint8_t> clr_data;
    auto type = static_cast<HSMns::ENCRYPTION_ALGORITHM_TYPE>(request->algo_type());
    HSMns::Ident id(request->my_id().id());
    uint32_t key_id = request->key_id();
    auto &hsm = HSMns::getInstance();
    HSMns::HSM_STATUS status = hsm.decrypt(
        enc_data,
        clr_data,
        type,
        id,
        key_id);
    if (status == HSMns::HSM_STATUS::HSM_Good)
    {
        reply->set_clr_data(clr_data.data(), clr_data.size());
    }
    reply->set_status(static_cast<HSM_gRpc::HSM_STATUS>(status));
    return grpc::Status::OK;
}

Status HSM_RPCServiceImpl::Sign(ServerContext *context,
                                const SignRequest *request, SignReply *reply) 
{
    std::vector<uint8_t> message(request->message().begin(), request->message().end());
    std::vector<uint8_t> signature;
    auto algo_type = static_cast<HSMns::ENCRYPTION_ALGORITHM_TYPE>(request->algo_type());
    auto hash_type = static_cast<HASH_ALGORITHM_TYPE>(request->hash_type());
    HSMns::Ident myId;
    HSMns::KeyId key_id = request->key_id();
    auto &hsm = HSMns::getInstance();
    HSMns::HSM_STATUS status = hsm.signMessage(message, signature, algo_type, hash_type, myId, key_id);

    if (status == HSMns::HSM_STATUS::HSM_Good)
    {
        reply->set_signature(signature.data(), signature.size());
    }
    reply->set_status(static_cast<HSM_gRpc::HSM_STATUS>(status));
    return Status::OK;
}

Status HSM_RPCServiceImpl::Verify(ServerContext *context,
                                    const VerifyRequest *request, VerifyReply *reply) 
{
    std::vector<uint8_t> message(request->message().begin(), request->message().end());
    std::vector<uint8_t> signature(request->signature().begin(), request->signature().end());
    auto algo_type = static_cast<HSMns::ENCRYPTION_ALGORITHM_TYPE>(request->algo_type());
    auto hash_type = static_cast<HASH_ALGORITHM_TYPE>(request->hash_type());
    HSMns::Ident my_id(request->my_id().id());
    HSMns::KeyId key_id = request->key_id();
    bool needPrivilege = request->need_privilege();
    auto &hsm = HSMns::getInstance();
    HSMns::HSM_STATUS status = hsm.verify(message, signature, algo_type, hash_type, my_id, key_id, needPrivilege);

    reply->set_status(static_cast<HSM_gRpc::HSM_STATUS>(status));
    return Status::OK;
}

Status HSM_RPCServiceImpl::GenKey(ServerContext *context,
                                    const GenKeyRequest *request, GenKeyReply *reply) 
{
    HSMns::Ident my_id(request->my_id().id());
    auto algo_type = static_cast<HSMns::ENCRYPTION_ALGORITHM_TYPE>(request->algo_type());
    uint32_t bits = request->bits();

    HSMns::KeyId key_id;

    auto &hsm = HSMns::getInstance();
    HSMns::HSM_STATUS status = hsm.create_key_and_get_id(my_id, key_id, algo_type, bits);

    if (status == HSMns::HSM_STATUS::HSM_Good)
    {
        reply->set_key_id(key_id);
    }
    reply->set_status(static_cast<HSM_gRpc::HSM_STATUS>(status));
    return Status::OK;
}

HSMServer::HSMServer(std::string const &server_address, std::shared_ptr<grpc::ServerCredentials> credentials)
{
    static bool grpc_initialized = []
    {
        grpc::EnableDefaultHealthCheckService(true);
        grpc::reflection::InitProtoReflectionServerBuilderPlugin();
        return true;
    }();

    grpc::ServerBuilder builder;
    // Listen on the given address without any authentication mechanism.
    builder.AddListeningPort(server_address, credentials);
    // Register "service" as the instance through which we'll communicate with
    // clients. In this case it corresponds to an *synchronous* service.
    builder.RegisterService(&service);

    // Finally assemble the server.
    this->server = builder.BuildAndStart();
    std::cout << "Server listening on " << server_address << std::endl;
}

void HSMServer::Wait()
{
    if (server)
    {
        server->Wait();
    }
}

void HSMServer::shutdown()
{
    if (server)
    {
        server->Shutdown();
    }
}

} // namespace HSMns 
} // namespace RPC 
