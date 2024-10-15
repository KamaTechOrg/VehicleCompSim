#pragma once

#include "../include/HSM.h"

#include <iostream>
#include <memory>
#include <string>

#include "hsm.grpc.pb.h"


namespace HSMns {
namespace RPC {
/**
 * @class HSM_GRPC_Client
 * @brief Implementation of the HSM interface over gRPC.
 */
class HSM_GRPC_Client : public HSM_Interface {
public:
    /**
     * @brief Constructor
     * @param channel The gRPC channel to use to connect to the server.
     */
    explicit HSM_GRPC_Client(std::shared_ptr<grpc::Channel> channel);

    /**
     * @brief Constructor
     * @param host The hostname:port of the server to connect to.
     */
    explicit HSM_GRPC_Client(std::string const& host = "localhost:50051");

    HSM_STATUS create_key_and_get_id(
            const Ident &myId,
            KeyId &keyId,
            ENCRYPTION_ALGORITHM_TYPE type,
            int bits = 512) override;

    HSM_STATUS encrypt(
        const std::vector<u_int8_t> &message,
        std::vector<u_int8_t> &encrypted_message,
        ENCRYPTION_ALGORITHM_TYPE type,
        const Ident &myId,
        const KeyId &keyId,
        bool needPrivilege = true) const override;

    HSM_STATUS decrypt(
        const std::vector<u_int8_t> &message,
        std::vector<u_int8_t> &decrypted_message,
        ENCRYPTION_ALGORITHM_TYPE type,
        const Ident &myId,
        const KeyId &keyId) const override;

    HSM_STATUS signMessage(
        const std::vector<u_int8_t> &message,
        std::vector<u_int8_t> &signature,
        ENCRYPTION_ALGORITHM_TYPE sigAlg,
        HASH_ALGORITHM_TYPE hashAlg,
        const Ident &myId,
        const KeyId &keyId) const override;

    HSM_STATUS verify(
        const std::vector<u_int8_t> &message,
        const std::vector<u_int8_t> &signature,
        ENCRYPTION_ALGORITHM_TYPE sigAlg,
        HASH_ALGORITHM_TYPE hashAlg,
        const Ident &myId,
        const KeyId &keyId,
        bool needPrivilege = true) const override;

private:
    std::unique_ptr<HSM_gRpc::HSM_RPC::Stub> stub_;
};

} // namespace RPC
} // namespace HSMns
