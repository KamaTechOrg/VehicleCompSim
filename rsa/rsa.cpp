#include "rsa.h"

void RSA::generate_keys(unsigned long long int& publicKey, unsigned long long int& privateKey, unsigned long long int& modulus, int bits) {
    unsigned long long int p = generateLargePrime(bits / 2);
    unsigned long long int q = generateLargePrime(bits / 2);
    unsigned long long int n = p * q;
    unsigned long long int phi = (p - 1) * (q - 1);
    unsigned long long int e = 65537;

    while (gcd(e, phi) != 1) {
        e = generateLargePrime(16);
    }

    unsigned long long int d = modInverse(e, phi);
    publicKey = e;
    privateKey = d;
    modulus = n;
}

bool RSA::isPrime(unsigned long long n, int k) {
    if (n <= 1 || n == 4) return false;
    if (n <= 3) return true;

    unsigned long long d = n - 1;
    while (d % 2 == 0)
        d /= 2;

    for (int i = 0; i < k; i++)
        if (!millerTest(d, n))
            return false;

    return true;
}

unsigned long long RSA::power(unsigned long long x, unsigned long long y, unsigned long long p) {
    unsigned long long res = 1;
    x = x % p;
    while (y > 0) {
        if (y & 1)
            res = (res * x) % p;
        y = y >> 1;
        x = (x * x) % p;
    }
    return res;
}

bool RSA::millerTest(unsigned long long d, unsigned long long n) {
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<unsigned long long> dis(2, n - 2);
    unsigned long long a = dis(gen);
    unsigned long long x = power(a, d, n);

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

unsigned long long RSA::generateRandomNumber(int bits) {
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<unsigned long long> dis(0, (1ULL << (bits - 1)) - 1);
    unsigned long long number = dis(gen) | (1ULL << (bits - 1)) | 1;
    return number;
}

unsigned long long RSA::generateLargePrime(int bits) {
    unsigned long long prime;
    int k = 40; // Number of iterations for Miller-Rabin test
    do {
        prime = generateRandomNumber(bits);
    } while (!isPrime(prime, k));
    return prime;
}

unsigned long long RSA::gcd(unsigned long long a, unsigned long long b) {
    while (b != 0) {
        unsigned long long t = b;
        b = a % b;
        a = t;
    }
    return a;
}

unsigned long long RSA::modInverse(unsigned long long a, unsigned long long m) {
    unsigned long long m0 = m, t, q;
    unsigned long long x0 = 0, x1 = 1;

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

unsigned long long RSA::encrypt(unsigned long long int message, unsigned long long int publicKey, unsigned long long int modulus) {
    return power(message, publicKey, modulus);
}

unsigned long long RSA::decrypt(unsigned long long int message, unsigned long long int privateKey, unsigned long long int modulus) {
    return power(message, privateKey, modulus);
}

std::string RSA::encrypt_string(std::string message, unsigned long long int publicKey, unsigned long long int modulus) {
    std::string result;
    for (char c : message) {
        result += std::to_string(encrypt(c, publicKey, modulus)) + " ";
    }
    return result;
}

std::string RSA::decrypt_string(std::string encrypted_message, unsigned long long int privateKey, unsigned long long int modulus) {
    std::string result;
    std::stringstream ss(encrypted_message);
    unsigned long long encrypted_char;
    while (ss >> encrypted_char) {
        result += static_cast<char>(decrypt(encrypted_char, privateKey, modulus));
    }
    return result;
}

