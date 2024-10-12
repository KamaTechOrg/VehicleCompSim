#include <string>
#include <gtest/gtest.h>
#include "hsm_grpc_client.hpp"
#include "HSM.h"

#include "AES_API.hpp"
#include "hsm_grpc_server.hpp"

using namespace HSMns;


class API_TEST : public testing::Test {
 protected:
    static std::unique_ptr<HSMns::RPC::HSMServer>  server;

    static void SetUpTestSuite() {
        server = std::make_unique<HSMns::RPC::HSMServer>("localhost:50051");
    }

    static void TearDownTestSuite() {
    if(server) 
        server->shutdown();
    }

    void TearDown() override { 
        HSM::resetInstance();
    }
};


std::unique_ptr<HSMns::RPC::HSMServer> API_TEST::server;


TEST_F(API_TEST, get_kye_rsa_64_bit)
{
    HSMns::RPC::HSM_GRPC_Client hsm;
    Ident myId = Ident();
    KeyId keyId;
    HSM_STATUS status = hsm.create_key_and_get_id(myId, keyId, ENCRYPTION_ALGORITHM_TYPE::RSA, 64);
    EXPECT_EQ(status, HSM_STATUS::HSM_Good);
    std::string massage = "hello world";
    std::vector<u_char> encrypted_massage;
    status = hsm.encrypt(std::vector<u_char>(massage.begin(), massage.end()), encrypted_massage, ENCRYPTION_ALGORITHM_TYPE::RSA, myId, keyId);
    EXPECT_EQ(status, HSM_STATUS::HSM_Good);
    std::vector<u_char> decrypted_massage;
    status = hsm.decrypt(encrypted_massage, decrypted_massage, ENCRYPTION_ALGORITHM_TYPE::RSA, myId, keyId);
    EXPECT_EQ(status, HSM_STATUS::HSM_Good);
    EXPECT_EQ(std::string(decrypted_massage.begin(), decrypted_massage.end()), massage);
}
TEST_F(API_TEST, get_kye_rsa_128_bit)
{
    HSMns::RPC::HSM_GRPC_Client hsm;
    Ident myId = Ident();
    KeyId keyId;
    HSM_STATUS status = hsm.create_key_and_get_id(myId, keyId, ENCRYPTION_ALGORITHM_TYPE::RSA, 128);
    EXPECT_EQ(status, HSM_STATUS::HSM_Good);
    std::string massage = "hello world";
    std::vector<u_char> encrypted_massage;
    status = hsm.encrypt(std::vector<u_char>(massage.begin(), massage.end()), encrypted_massage, ENCRYPTION_ALGORITHM_TYPE::RSA, myId, keyId);
    EXPECT_EQ(status, HSM_STATUS::HSM_Good);
    std::vector<u_char> decrypted_massage;
    status = hsm.decrypt(encrypted_massage, decrypted_massage, ENCRYPTION_ALGORITHM_TYPE::RSA, myId, keyId);
    EXPECT_EQ(status, HSM_STATUS::HSM_Good);
    EXPECT_EQ(std::string(decrypted_massage.begin(), decrypted_massage.end()), massage);
}

TEST_F(API_TEST, rsa_encrypt_without_user_id)
{
    HSMns::RPC::HSM_GRPC_Client hsm;
    KeyId keyId;
    Ident myId = Ident();
    HSM_STATUS status = hsm.create_key_and_get_id(myId, keyId, ENCRYPTION_ALGORITHM_TYPE::RSA, 64);
    EXPECT_EQ(status, HSM_STATUS::HSM_Good);
    std::string massage = "hello world";
    std::vector<u_char> encrypted_massage;
    status = hsm.encrypt(std::vector<u_char>(massage.begin(), massage.end()), encrypted_massage, ENCRYPTION_ALGORITHM_TYPE::RSA, Ident{"notMyId"}, keyId, false);
    EXPECT_EQ(status, HSM_STATUS::HSM_Good);
    std::vector<u_char> decrypted_massage;
    status = hsm.decrypt(encrypted_massage, decrypted_massage, ENCRYPTION_ALGORITHM_TYPE::RSA, myId, keyId);
    EXPECT_EQ(status, HSM_STATUS::HSM_Good);
    EXPECT_EQ(std::string(decrypted_massage.begin(), decrypted_massage.end()), massage);
}

TEST_F(API_TEST, get_kye_invalid_type)
{
    HSMns::RPC::HSM_GRPC_Client hsm;
    Ident myId = Ident();
    KeyId keyId;
    HSM_STATUS status = hsm.create_key_and_get_id(myId, keyId, ENCRYPTION_ALGORITHM_TYPE::NoAlg, 128);
    EXPECT_EQ(status, HSM_STATUS::HSM_InvalidAlg);
}

TEST_F(API_TEST, aes_all_types_many_msg_sizes)
{
    std::array<ENCRYPTION_ALGORITHM_TYPE, 9> aes_types = {
        ENCRYPTION_ALGORITHM_TYPE::AES_128_ECB,
        ENCRYPTION_ALGORITHM_TYPE::AES_128_CBC,
        ENCRYPTION_ALGORITHM_TYPE::AES_128_CTR,
        ENCRYPTION_ALGORITHM_TYPE::AES_192_ECB,
        ENCRYPTION_ALGORITHM_TYPE::AES_192_CBC,
        ENCRYPTION_ALGORITHM_TYPE::AES_192_CTR,
        ENCRYPTION_ALGORITHM_TYPE::AES_256_ECB,
        ENCRYPTION_ALGORITHM_TYPE::AES_256_CBC,
        ENCRYPTION_ALGORITHM_TYPE::AES_256_CTR};

    std::vector<size_t> msg_sizes{0, 1, 2, 15, 16, 17, 31, 32, 33, 1234, 56789, 101112};
    srand(0);

    for (auto msg_size : msg_sizes)
    {
        std::vector<uint8_t> msg(msg_size);
        for (auto &c : msg)
            c = rand();

        for (ENCRYPTION_ALGORITHM_TYPE aes_type : aes_types)
        {
            std::string failure_msg = "fail on aes_type: " + std::to_string(aes_type) + ",  msg_size: " + std::to_string(msg_size);
            HSMns::RPC::HSM_GRPC_Client hsm;
            KeyId keyId;
            Ident myId;
            HSM_STATUS status = hsm.create_key_and_get_id(myId, keyId, aes_type);
            ASSERT_EQ(status, HSM_STATUS::HSM_Good) << failure_msg;

            std::vector<uint8_t> enc_msg;
            std::vector<uint8_t> clr_msg;
            ASSERT_EQ(hsm.encrypt(msg, enc_msg, aes_type, myId, keyId), HSM_STATUS::HSM_Good) << failure_msg;
            ASSERT_EQ(hsm.decrypt(enc_msg, clr_msg, aes_type, myId, keyId), HSM_STATUS::HSM_Good) << failure_msg;
            EXPECT_EQ(msg, clr_msg) << failure_msg;
        }
    }
}

TEST_F(API_TEST, printAESKye)
{
    ENCRYPTION_ALGORITHM_TYPE aes_type = ENCRYPTION_ALGORITHM_TYPE::AES_128_ECB;
    std::vector<uint8_t> key;
    EXPECT_EQ(AES::generateAndPrintKey(key, aes_type), HSM_STATUS::HSM_Good);
}