#include "hsm_grpc_client.hpp"

#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>

#include "hsm.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
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

using HSM_gRpc::HSM_RPC;


namespace HSMns {
namespace RPC {

HSM_GRPC_Client::HSM_GRPC_Client(std::shared_ptr<Channel> channel)
    : stub_(HSM_gRpc::HSM_RPC::NewStub(channel)) {}

HSM_GRPC_Client::HSM_GRPC_Client(std::string const &host)
    : HSM_GRPC_Client(grpc::CreateChannel(host, grpc::InsecureChannelCredentials()))
{}

HSM_STATUS HSM_GRPC_Client::create_key_and_get_id(const Ident &my_id, KeyId &keyId, ENCRYPTION_ALGORITHM_TYPE type, int bits)
{
    GenKeyRequest request;
    request.mutable_my_id()->set_id(my_id.toString());
    request.set_algo_type(static_cast<HSM_gRpc::ENCRYPTION_ALGORITHM_TYPE>(type));
    request.set_bits(bits);

    GenKeyReply reply;
    ClientContext context;
    Status status = stub_->GenKey(&context, request, &reply);

    if (status.ok()) {
        keyId = reply.key_id();
        return static_cast<HSMns::HSM_STATUS>(reply.status());
    }

    return HSM_STATUS::HSM_InternalErr;
}

HSM_STATUS HSM_GRPC_Client::encrypt(
    const std::vector<u_int8_t> &clr_data,
    std::vector<u_int8_t> &enc_data,
    ENCRYPTION_ALGORITHM_TYPE type,
    const Ident &my_id,
    const KeyId &keyId,
    bool needPrivilege) const
{
    EncryptRequest request;
    request.set_clr_data(std::string_view{reinterpret_cast<const char *>(clr_data.data()), clr_data.size()});

    request.set_algo_type(static_cast<HSM_gRpc::ENCRYPTION_ALGORITHM_TYPE>(type));
    request.mutable_my_id()->set_id(my_id.toString());
    request.set_key_id(keyId);
    request.set_need_privilege(needPrivilege);

    EncryptReply reply;
    ClientContext context;

    Status status = stub_->Encrypt(&context, request, &reply);

    if (status.ok()) {
        enc_data = {reply.enc_data().begin(), reply.enc_data().end()};
        return static_cast<HSMns::HSM_STATUS>(reply.status());
    }

    return HSM_STATUS::HSM_InternalErr;
}

HSM_STATUS HSM_GRPC_Client::decrypt(
    const std::vector<u_int8_t> &enc_data,
    std::vector<u_int8_t> &clr_data,
    ENCRYPTION_ALGORITHM_TYPE type,
    const Ident &my_id,
    const KeyId &keyId) const
{
    DecryptRequest request;
    request.set_enc_data(std::string_view{reinterpret_cast<const char *>(enc_data.data()), enc_data.size()});
    request.set_algo_type(static_cast<HSM_gRpc::ENCRYPTION_ALGORITHM_TYPE>(type));
    request.mutable_my_id()->set_id(my_id.toString());
    request.set_key_id(keyId);

    DecryptReply reply;
    ClientContext context;

    Status status = stub_->Decrypt(&context, request, &reply);

    if (status.ok()) {
        clr_data = std::vector<uint8_t>{reply.clr_data().begin(), reply.clr_data().end()};
        return static_cast<HSMns::HSM_STATUS>(reply.status());
    }

    return HSM_STATUS::HSM_InternalErr;
}

HSM_STATUS HSM_GRPC_Client::signMessage(
    const std::vector<u_int8_t> &message,
    std::vector<u_int8_t> &signature,
    ENCRYPTION_ALGORITHM_TYPE sigAlg,
    HASH_ALGORITHM_TYPE hashAlg,
    const Ident &my_id,
    const KeyId &keyId) const
{
    SignRequest request;
    request.set_message(std::string_view{reinterpret_cast<const char *>(message.data()), message.size()});
    request.set_algo_type(static_cast<HSM_gRpc::ENCRYPTION_ALGORITHM_TYPE>(sigAlg));
    request.set_hash_type(static_cast<HSM_gRpc::HASH_ALGORITHM_TYPE>(hashAlg));
    request.mutable_my_id()->set_id(my_id.toString());
    request.set_key_id(keyId);

    SignReply reply;
    ClientContext context;

    Status status = stub_->Sign(&context, request, &reply);

    if (status.ok()) {
        signature = {reply.signature().begin(), reply.signature().end()};
        return static_cast<HSMns::HSM_STATUS>(reply.status());
    }

    return HSM_STATUS::HSM_InternalErr;
}

HSM_STATUS HSM_GRPC_Client::verify(
    const std::vector<u_int8_t> &message,
    const std::vector<u_int8_t> &signature,
    ENCRYPTION_ALGORITHM_TYPE sigAlg,
    HASH_ALGORITHM_TYPE hashAlg,
    const Ident &my_id,
    const KeyId &keyId,
    bool need_privilege) const
{
    VerifyRequest request;
    request.set_message(std::string_view{reinterpret_cast<const char *>(message.data()), message.size()});
    request.set_signature(std::string_view{reinterpret_cast<const char *>(signature.data()), signature.size()});
    request.set_algo_type(static_cast<HSM_gRpc::ENCRYPTION_ALGORITHM_TYPE>(sigAlg));
    request.set_hash_type(static_cast<HSM_gRpc::HASH_ALGORITHM_TYPE>(hashAlg));
    request.mutable_my_id()->set_id(my_id.toString());
    request.set_key_id(keyId);
    request.set_need_privilege(need_privilege);

    VerifyReply reply;
    ClientContext context;

    Status status = stub_->Verify(&context, request, &reply);

    if (status.ok()) {
        return static_cast<HSMns::HSM_STATUS>(reply.status());
    }

    return HSM_STATUS::HSM_InternalErr;
}

} // namespace RPC
} // namespace HSMns
