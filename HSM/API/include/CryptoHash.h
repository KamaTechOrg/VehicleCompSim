class CryptoHash
{
private:
    /* data */
public:
    CryptoHash(/* args */);
    ~CryptoHash();

    virtual uint32_t append(const std::ByteArray & data) = 0;
    virtual uint32_t finalize() = 0;
    virtual uint32_t getResult(std::ByteArray & result) = 0;
};

CryptoHash::CryptoHash(/* args */)
{
}

CryptoHash::~CryptoHash()
{
}
