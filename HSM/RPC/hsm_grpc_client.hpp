#pragma once

#include "../include/HSM.h"

#include <iostream>
#include <memory>
#include <string>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"

#include <grpcpp/grpcpp.h>

#include "hsm.grpc.pb.h"


namespace HSMns {
namespace RPC {

class HSM_GRPC_Client {
public:
    explicit HSM_GRPC_Client(std::shared_ptr<Channel> channel);
    explicit HSM_GRPC_Client(std::string const& host);
    HSM_STATUS encrypt(
        const std::vector<u_int8_t> &message,
        std::vector<u_int8_t> &encrypted_message,
        ENCRYPTION_ALGORITHM_TYPE type,
        const Ident &myId,
        const KeyId &keyId,
        bool needPrivilege = true) const;

    HSM_STATUS decrypt(
        const std::vector<u_int8_t> &message,
        std::vector<u_int8_t> &decrypted_message,
        ENCRYPTION_ALGORITHM_TYPE type,
        const Ident &myId,
        const KeyId &keyId) const;

    HSM_STATUS signMessage(
        const std::vector<u_int8_t> &message,
        std::vector<u_int8_t> &signature,
        ENCRYPTION_ALGORITHM_TYPE sigAlg,
        HASH_ALGORITHM_TYPE hashAlg,
        const Ident &myId,
        const KeyId &keyId) const;

    HSM_STATUS verify(
        const std::vector<u_int8_t> &message,
        const std::vector<u_int8_t> &signature,
        ENCRYPTION_ALGORITHM_TYPE sigAlg,
        HASH_ALGORITHM_TYPE hashAlg,
        const Ident &myId,
        const KeyId &keyId,
        bool needPrivilege = true) const;


private:
    class HSM_RPC::Stub;
    std::unique_ptr<HSM_RPC::Stub> stub_;
};

} // namespace RPC
} // namespace HSMns
