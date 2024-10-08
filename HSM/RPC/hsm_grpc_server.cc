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

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/strings/str_format.h"

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>


#include "hsm.grpc.pb.h"


#include "HSM.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using HSM_gRpc::HSM_RPC;
using HSM_gRpc::EncryptReply;
using HSM_gRpc::EncryptRequest;
using HSM_gRpc::DecryptReply;
using HSM_gRpc::DecryptRequest;
using HSM_gRpc::SignReply;
using HSM_gRpc::SignRequest;
using HSM_gRpc::VerifyReply;
using HSM_gRpc::VerifyRequest;

ABSL_FLAG(uint16_t, port, 50051, "Server port for the service");

// Logic and data behind the server's behavior.
class HSM_RPCServiceImpl final : public HSM_RPC::Service {
  Status Encrypt(ServerContext* context, 
      const EncryptRequest* request, EncryptReply* reply) override {
        std::vector<uint8_t> clr_data(request->clr_data().begin(), request->clr_data().end());
        std::vector<uint8_t> enc_data;
        auto type = static_cast<HSMns::ENCRYPTION_ALGORITHM_TYPE>(request->algo_type());
        HSMns::Ident id(request->my_id().id());
        uint32_t key_id = request->key_id();
        bool needPrivilege = request->need_privilege();
        auto& hsm = HSMns::getInstance();
        HSMns::HSM_STATUS status = hsm.encrypt(
          clr_data,
          enc_data,
          type,
          id,
          key_id,
          needPrivilege
        );
        reply->set_status(static_cast<HSM_gRpc::HSM_STATUS>(status));
        if(status == HSMns::HSM_STATUS::HSM_Good){
          reply->set_enc_data(enc_data.data(), enc_data.size());
          return Status::OK;
        }
        return Status(grpc::StatusCode::INTERNAL, "HSM_STATUS: " + std::to_string(status));
  }

  Status Decrypt(ServerContext* context, 
      const DecryptRequest* request, DecryptReply* reply) override {
        std::vector<uint8_t> enc_data(request->enc_data().begin(), request->enc_data().end());
        std::vector<uint8_t> clr_data;
        auto type = static_cast<HSMns::ENCRYPTION_ALGORITHM_TYPE>(request->algo_type());
        HSMns::Ident id(request->my_id().id());
        uint32_t key_id = request->key_id();
        auto& hsm = HSMns::getInstance();
        HSMns::HSM_STATUS status = hsm.decrypt(
          enc_data,
          clr_data,
          type,
          id,
          key_id
        );
        reply->set_status(static_cast<HSM_gRpc::HSM_STATUS>(status));
        if(status == HSMns::HSM_STATUS::HSM_Good){
          reply->set_clr_data(clr_data.data(), clr_data.size());
          return Status::OK;
        }
        return Status(grpc::StatusCode::INTERNAL, "HSM_STATUS: " + std::to_string(status));
  }


Status Sign(ServerContext* context, 
      const SignRequest* request, SignReply* reply) override {
        std::vector<uint8_t> message(request->message().begin(), request->message().end());
        std::vector<uint8_t> signature;
        auto algo_type = static_cast<HSMns::ENCRYPTION_ALGORITHM_TYPE>(request->algo_type());
        auto hash_type = static_cast<HASH_ALGORITHM_TYPE>(request->hash_type());
        HSMns::Ident myId;
        HSMns::KeyId key_id = request->key_id();
        auto& hsm = HSMns::getInstance();
        HSMns::HSM_STATUS status = hsm.signMessage(message, signature, algo_type, hash_type, myId, key_id);

        reply->set_status(static_cast<HSM_gRpc::HSM_STATUS>(status));
        if(status == HSMns::HSM_STATUS::HSM_Good){
          reply->set_signature(signature.data(), signature.size());
          return Status::OK;
        }
        return Status(grpc::StatusCode::INTERNAL, "HSM_STATUS: " + std::to_string(status));
  }


  Status Verify(ServerContext* context, 
      const VerifyRequest* request, VerifyReply* reply) override {
        std::vector<uint8_t> message(request->message().begin(), request->message().end());
        std::vector<uint8_t> signature(request->signature().begin(), request->signature().end());
        auto algo_type = static_cast<HSMns::ENCRYPTION_ALGORITHM_TYPE>(request->algo_type());
        auto hash_type = static_cast<HASH_ALGORITHM_TYPE>(request->hash_type());
        HSMns::Ident my_id;
        HSMns::KeyId key_id = request->key_id();
        bool needPrivilege = request->need_privilege();
        auto& hsm = HSMns::getInstance();
        HSMns::HSM_STATUS status = hsm.verify(message, signature, algo_type, hash_type, my_id, key_id, needPrivilege);

        reply->set_status(static_cast<HSM_gRpc::HSM_STATUS>(status));
        if(status == HSMns::HSM_STATUS::HSM_Good){
          return Status::OK;
        }
        return Status(grpc::StatusCode::INTERNAL, "HSM_STATUS: " + std::to_string(status));
  }
};

void RunServer(uint16_t port) {
  std::string server_address = absl::StrFormat("0.0.0.0:%d", port);
  HSM_RPCServiceImpl service;

  grpc::EnableDefaultHealthCheckService(true);
  grpc::reflection::InitProtoReflectionServerBuilderPlugin();
  ServerBuilder builder;
  // Listen on the given address without any authentication mechanism.
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  // Register "service" as the instance through which we'll communicate with
  // clients. In this case it corresponds to an *synchronous* service.
  builder.RegisterService(&service);
  // Finally assemble the server.
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;

  // Wait for the server to shutdown. Note that some other thread must be
  // responsible for shutting down the server for this call to ever return.
  server->Wait();
}

int main(int argc, char** argv) {
  absl::ParseCommandLine(argc, argv);
  RunServer(absl::GetFlag(FLAGS_port));
  return 0;
}
