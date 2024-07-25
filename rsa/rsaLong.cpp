#include "rsaLong.h"

void RSA::generate_keys(BigNum& publicKey, BigNum& privateKey, BigNum& modulus, int bits) {
    BigNum p = generateLargePrime(bits / 2);
    BigNum q = generateLargePrime(bits / 2);
    BigNum n = p * q;
    BigNum phi = (p - 1) * (q - 1);
    BigNum e = 65537;

    while (gcd(e, phi) != 1) {
        e = generateLargePrime(16);
    }

    BigNum d = modInverse(e, phi);
    publicKey = e;
    privateKey = d;
    modulus = n;
}

bool RSA::isPrime(BigNum n, int k) {
    if (n <= 1 || n == 4) return false;
    if (n <= 3) return true;

    BigNum d = n - 1;
    while (d % 2 == 0)
        d /= 2;

    for (int i = 0; i < k; i++)
        if (!millerTest(d, n))
            return false;

    return true;
}

BigNum RSA::power(BigNum x, BigNum y, BigNum p) {
    BigNum res = 1;
    x = x % p;
    while (y > 0) {
        if (y & 1)
            res = (res * x) % p;
        y = y >> 1;
        x = (x * x) % p;
    }
    return res;
}

bool RSA::millerTest(BigNum d, BigNum n) {
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<BigNum> dis(2, n - 2);
    BigNum a = dis(gen);
    BigNum x = power(a, d, n);

    if (x == 1 || x == n - 1)
        return true;

    while (d != n - 1) {
        x = (x * x) % n;
        d *= 2;

        if (x == 1) return false;
        if (x == n - 1) return true;
    }
    return false;
}

BigNum RSA::generateRandomNumber(int bits) {
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<BigNum> dis(0, (1ULL << (bits - 1)) - 1);
    BigNum number = dis(gen) | (1ULL << (bits - 1)) | 1;
    return number;
}

BigNum RSA::generateLargePrime(int bits) {
    BigNum prime;
    int k = 40; // Number of iterations for Miller-Rabin test
    do {
        prime = generateRandomNumber(bits);
    } while (!isPrime(prime, k));
    return prime;
}

BigNum RSA::gcd(BigNum a, BigNum b) {
    while (b != 0) {
        BigNum t = b;
        b = a % b;
        a = t;
    }
    return a;
}

BigNum RSA::modInverse(BigNum a, BigNum m) {
    BigNum m0 = m, t, q;
    BigNum x0 = 0, x1 = 1;

    if (m == 1)
        return 0;

    while (a > 1) {
        q = a / m;
        t = m;
        m = a % m;
        a = t;
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }

    if (x1 < 0)
        x1 += m0;

    return x1;
}

BigNum RSA::encrypt(BigNum message, BigNum publicKey, BigNum modulus) {
    return power(message, publicKey, modulus);
}

BigNum RSA::decrypt(BigNum message, BigNum privateKey, BigNum modulus) {
    return power(message, privateKey, modulus);
}

std::string RSA::encrypt_string(std::string message, BigNum publicKey, BigNum modulus) {
    std::string result;
    for (char c : message) {
        result += std::to_string(encrypt(c, publicKey, modulus)) + " ";
    }
    return result;
}

std::string RSA::decrypt_string(std::string encrypted_message, BigNum privateKey, BigNum modulus) {
    std::string result;
    std::stringstream ss(encrypted_message);
    BigNum encrypted_char;
    while (ss >> encrypted_char) {
        result += static_cast<char>(decrypt(encrypted_char, privateKey, modulus));
    }
    return result;
}

