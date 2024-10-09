#include "hsm_grpc_client.hpp"

#include <iostream>
#include <memory>
#include <string>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"

#include <grpcpp/grpcpp.h>

#include "hsm.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using HSM_gRpc::DecryprtReply;
using HSM_gRpc::DecryprtRequest;
using HSM_gRpc::EncryptReply;
using HSM_gRpc::EncryptRequest;
using HSM_gRpc::HSM_RPC;
using HSM_gRpc::SignReply;
using HSM_gRpc::SignRequest;
using HSM_gRpc::VerifyReply;
using HSM_gRpc::VerifyRequest;



namespace HSMns {
namespace RPC {


HSM_GRPC_Client::HSM_GRPC_Client(std::shared_ptr<Channel> channel)
    : stub_(HSM_RPC::NewStub(channel)) {}


HSM_STATUS HSM_GRPC_Client::encrypt(
    const std::vector<u_int8_t> &clr_data,
    std::vector<u_int8_t> &enc_data,
    ENCRYPTION_ALGORITHM_TYPE type,
    const Ident &myId,
    const KeyId &keyId,
    bool needPrivilege = true) const
{
    EncryptRequest request;
    request.set_clr_data(clr_data().begin(), clr_data().end());
    request.set_algo_type(type);
    request.set_my_id(my_id().toString());
    request.set_key_id(keyId);
    request.set_need_privilege(need_privilege);

    EncryptReply reply;
    ClientContext context;

    Status status = stub_->Encrypt(&context, request, &reply);

    if (status.ok())
    {
        enc_data = {reply.enc_data().begin(), reply.enc_data().end()};
        return reply.status();
    }
    else
    {
        return HSM_STATUS::HSM_InternalErr;
    }
}

HSM_STATUS HSM_GRPC_Client::decrypt(
    const std::vector<u_int8_t> &enc_data,
    std::vector<u_int8_t> &clr_data,
    ENCRYPTION_ALGORITHM_TYPE type,
    const Ident &myId,
    const KeyId &keyId) const
{
    DecryptRequest request;
    request.set_enc_data(enc_data().begin(), enc_data().end());
    request.set_algo_type(type);
    request.set_my_id(my_id().toString());
    request.set_key_id(keyId);

    DecryptReply reply;
    ClientContext context;

    Status status = stub_->Decrypt(&context, request, &reply);

    if (status.ok())
    {
        clr_data = {reply.clr_data().begin(), reply.clr_data().end()};
        return reply.status();
    }
    else
    {
        return HSM_STATUS::HSM_InternalErr;
    }
}

HSM_STATUS signMessage(
    const std::vector<u_int8_t> &message,
    std::vector<u_int8_t> &signature,
    ENCRYPTION_ALGORITHM_TYPE sigAlg,
    HASH_ALGORITHM_TYPE hashAlg,
    const Ident &myId,
    const KeyId &keyId) const
{
    SignRequest request;
    request.set_message(message().begin(), message().end());
    request.set_algo_type(sigAlg);
    request.set_hash_type(hashAlg);
    request.set_my_id(my_id().toString());
    request.set_key_id(keyId);

    SignReply reply;
    ClientContext context;

    Status status = stub_->Sign(&context, request, &reply);

    if (status.ok())
    {
        signature = {reply.signature().begin(), reply.signature().end()};
        return reply.status();
    }
    else
    {
        return HSM_STATUS::HSM_InternalErr;
    }
}

HSM_STATUS verify(
    const std::vector<u_int8_t> &message,
    const std::vector<u_int8_t> &signature,
    ENCRYPTION_ALGORITHM_TYPE sigAlg,
    HASH_ALGORITHM_TYPE hashAlg,
    const Ident &myId,
    const KeyId &keyId,
    bool needPrivilege = true) const
{
    VerifyRequest request;
    request.set_message(message().begin(), message().end());
    request.set_signature(signature().begin(), signature().end());
    request.set_algo_type(sigAlg);
    request.set_hash_type(hashAlg);
    request.set_my_id(my_id().toString());
    request.set_key_id(keyId);
    request.set_need_privilege(need_privilege);

    VerifyReply reply;
    ClientContext context;

    Status status = stub_->Verify(&context, request, &reply);

    if (status.ok())
    {
        return reply.status();
    }
    else
    {
        return HSM_STATUS::HSM_InternalErr;
    }
}

} // namespace RPC
} // namespace HSMns
