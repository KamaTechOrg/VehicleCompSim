#pragma once

#include <string>
#include <utility>
#include "RSA.h"
#include "aes.hpp"
#include "Signature.h"


class HSM
{
public:
    static std::pair<std::vector<u_char>, std::vector<u_char>> generateAsymmetricKeys(ENCRYPTION_ALGORITHM_TYPE type, int bits = 512);

    static std::vector<u_char> generateSymmetricKey(ENCRYPTION_ALGORITHM_TYPE type, int bits = 128);

    static std::vector<u_char> encrypt(const std::vector<u_char> message, ENCRYPTION_ALGORITHM_TYPE type, const std::vector<u_char> key, int bits = 512);

    static std::vector<u_char> decrypt(const std::vector<u_char> message, ENCRYPTION_ALGORITHM_TYPE type, const std::vector<u_char> key, int bits = 512);

    static std::vector<u_char> signMessage(
        const std::vector<u_char> message, 
        ENCRYPTION_ALGORITHM_TYPE sigAlg, 
        ENCRYPTION_ALGORITHM_TYPE hashAlg,
        const std::vector<u_char> key
    );
    
    static bool verify(
        const std::vector<u_char> message,
        const std::vector<u_char> signature,
        ENCRYPTION_ALGORITHM_TYPE sigAlg,
        ENCRYPTION_ALGORITHM_TYPE hashAlg,
        const std::vector<u_char> key
    );
};

/*
egarding interfaces - we will need the low-level interfaces anyway, in order to do the job internally. If we decide to add something on top of this as more high-level, we can always do this afterwards, and hide the specific keys definitions and work with predefined configurations only

I started to define the interfaces (thiugh this better should be done with students as it is essentially the most important part the I can contribute), and realized that it's more proper to do it with templates. Should we use templates, or only old C++ inheritance, or maybe plain C API?

below is some approximate API in C++ without templates

So, let's assume we have a notation of crypto key
class CryptoKey {
};

and derivative
class EncryptionKey : public CryptoKey {};
class DecryptionKey : public CryptoKey {};

and also
class SigningKey : public CryptoKey {};
class VerificationKey : public CryptoKey {};

apparently, some types of keys will inherit from more that one, so they will have to handle the virtual inheritance in this case

Now, you have to define the algorithm. For complete fun, let's define a a base class for the algorithm

class CryptoAlgorithm {};
and defivatives
class EncryptionAlgorithm : public CryptoAlgorithm {};
class SigningAlgorithm : public CryptoAlgorithm {};
class SymmetricEncryptionAlgorithm : public EnryptionAlgorithm
class AES: public SymmetricEncryptionAlgorithm
class AES128: public AES

we will also need the same for hashes. Since all hashes have the same interface, this shoul be easy

class CryptoHash {
public:
      virtual uint32_t append(const std::ByteArray & data) = 0;
               virtual uint32_t finalize() = 0;
               virtual uint32_t getResult(std::ByteArray & result) = 0;
};

so that particular hashes will look like
class SHA256 : public CryptoHash {};

you can define the streams also as classes
class ChaningStream{
public:
  ChaningStream(SymmetricEncryptionAlgorithm algo);
  encrypt(const std::ByteArray & IV, const std::ByteArray & data, std::ByteArray & output) = 0;
  decrypt(const std::ByteArray & IV, const std::ByteArray & data, std::ByteArray & output) = 0;
}

class CBC : public ChaningStream {}

class AES_CBC : public CBC // THIS SHOULD BE DONE WITH TEMPLATES for generic solution

and typical flow of encryption of data will be
//create the cryptoKey
AES128 enckey (random(keylen));
AES128CBC encAlgo;
std::ByteArray IV(random(IV_LEN));
std::ByteArray output(data_len)
encAlgo.encrypt(encKey, IV, data, output)

*/