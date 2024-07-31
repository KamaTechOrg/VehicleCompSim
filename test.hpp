#pragma once
#include <iostream>
#include "BigNum/BigNum.h"
#include "rsa/RSA.h"



void testBigNum() {
    std::cout << "Testing BigNum..." << std::endl;

   

   
    

    
    

   
    

   
    


    


    std::cout << "Testing BigNum done" << std::endl;
}


void testBigNumThatAlreadyPassed() {


    //Test addition with int
    {
        BigNum a("1");
        BigNum b("f");
        BigNum c = a + b;
        std::string expected = "10";
        std::string result = c.toString();
        if (expected == result) {
            //std::cout << "BigNum addition with int passed" << std::endl;
        }
        else {
            std::cout << "BigNum addition with int failed" << std::endl;
        }
    }


    //Test subtraction with int
    {
        BigNum a("10");
        BigNum b("1");
        BigNum c = a - b;
        std::string expected = "f";
        std::string result = c.toString();
        if (expected == result) {
            //std::cout << "BigNum subtraction with int passed" << std::endl;
        }
        else {
            std::cout << "BigNum subtraction with int failed" << std::endl;
        }
    }

    //Test division with int
    {
        BigNum a("e");
        BigNum c = a / 4;
        std::string expected = "3";
        std::string result = c.toString();
        if (expected == result) {
            //std::cout << "BigNum division with int passed" << std::endl;
        }
        else {
            std::cout << "BigNum division with int failed" << std::endl;
            std::cout << "expected: " << expected << " result: " << result << std::endl;
        }
    }


    //Test modulus with int
    {
        BigNum a("b");
        BigNum c = a % 2;
        std::string expected = "1";
        std::string result = c.toString();
        if (expected == result) {
            //std::cout << "BigNum modulus with int passed" << std::endl;
        }
        else {
            std::cout << "BigNum modulus with int failed" << std::endl;
        }
    }



    //Test toString
    {
        BigNum a("f15e7");
        std::string expected = "f15e7";
        std::string result = a.toString();
        if (expected == result) {
            //std::cout << "BigNum toString passed" << std::endl;
        }
        else {
            std::cout << "BigNum toString failed" << std::endl;
        }
    }



	std::cout << "Testing BigNum that passes allrady..." << std::endl;
    // Test plain addition
    {
        BigNum a("1");
        BigNum b("f");
        BigNum c = a + b;
        std::string expected = "10"; // 1 + 15 = 16 in decimal, which is 10 in hex
        std::string result = c.toString();
        if (expected == result) {
            //std::cout << "BigNum plain addition passed" << std::endl;
        }
        else {
            std::cout << "BigNum plain addition failed" << std::endl;
        }
    }

    // Test plain subtraction
    {
        std::string expected = "f"; // 16 - 1 = 15 in decimal, which is f in hex
        BigNum d("10");
        BigNum e("1");
        BigNum f = d - e;
        std::string result = f.toString();
        if (expected == result) {
            //std::cout << "BigNum plain subtraction passed" << std::endl;
        }
        else {
            std::cout << "BigNum plain subtraction failed" << std::endl;
        }
    }

    // Test plain multiplication
    {
        std::string expected = "f"; // 5 * 3 = 15 in decimal, which is f in hex
        BigNum g("5");
        BigNum h("3");
        BigNum i = g * h;
        std::string result = i.toString();
        if (expected == result) {
            //std::cout << "BigNum plain multiplication passed" << std::endl;
        }
        else {
            std::cout << "BigNum plain multiplication failed" << std::endl;
        }
    }

    // test plain shift left
    {
        std::string expected = "78"; //1111 to 111 1000 is 78 in hex
        BigNum g("f");
        BigNum i = g << 3;
        std::string result = i.toString();
        if (expected == result) {
            //std::cout << "BigNum plain shift left passed" << std::endl;
        }
        else {
            std::cout << "BigNum plain shift left failed" << std::endl;
            std::cout << "expected: " << expected << ", result: " << result << std::endl;
        }
    }

    // test plain shift right
    {
        std::string expected = "1"; //1111 to 0001 is 1 in hex
        BigNum g("f");
        BigNum i = g >> 3;
        std::string result = i.toString();
        if (expected == result) {
            //std::cout << "BigNum plain shift right passed" << std::endl;
        }
        else {
            std::cout << "BigNum plain shift right failed" << std::endl;
            std::cout << "expected: " << expected << ", result: " << result << std::endl;
        }
    }


    // Test plain division
    {
        std::string expected = "4"; // 14 / 3 = 4 in decimal, which is 4 in hex
        BigNum j("e");
        BigNum k("3");
        BigNum l = j / k;
        std::string result = l.toString();
        if (expected == result) {
            //std::cout << "BigNum plain division passed" << std::endl;
        }
        else {
            std::cout << "BigNum plain division failed" << std::endl;
            std::cout << "expected: " << expected << ", result: " << result << std::endl;
        }
    }

    //Test plain modulus
    {
        std::string expected = "2"; // 14 % 3 = 2 in decimal, which is 2 in hex
        BigNum m("e");
        BigNum n("3");
        BigNum o = m % n;
        std::string result = o.toString();
        if (expected == result) {
            //std::cout << "BigNum plain modulus passed" << std::endl;
        }
        else {
            std::cout << "BigNum plain modulus failed" << std::endl;
        }
    }

    // Test  < operator with different sizes
    {
        BigNum i1("f");
        BigNum j2("fffffffffffff");


        if (i1 < j2) {
            //std::cout << "BigNum different sizes < operator passed" << std::endl;
        }
        else {
            std::cout << "BigNum different sizes < operator failed" << std::endl;
        }
    }

    // Test  >= operator with different sizes
    {
        BigNum k1("ffffffffffffffffff");
        BigNum l2("fff");

        if (k1 >= l2) {
            //std::cout << "BigNum different sizes >= operator passed" << std::endl;
        }
        else {
            std::cout << "BigNum different sizes >= operator failed" << std::endl;
        }
    }


   


    // Test 32-bit addition
    {
        std::string expected = "100000000"; // ffffffff + 1 = 100000000 in hex
        BigNum p("ffffffff");
        BigNum q("1");
        BigNum r = p + q;
        std::string result = r.toString();
        if (expected == result) {
            //std::cout << "BigNum 32-bit addition passed" << std::endl;
        }
        else {
            std::cout << "BigNum 32-bit addition failed" << std::endl;
			std::cout << "expected: " << expected << ", result: " << result << std::endl;
        }
    }

    // Test 32-bit subtraction
    {
        std::string expected = "fffffffe"; // ffffffff - 1 = fffffffe in hex
        BigNum s("ffffffff");
        BigNum t("1");
        BigNum u = s - t;
        std::string result = u.toString();
        if (expected == result) {
            //std::cout << "BigNum 32-bit subtraction passed" << std::endl;
        }
        else {
            std::cout << "BigNum 32-bit subtraction failed" << std::endl;
        }
    }

    // Test 32-bit multiplication
    {
        std::string expected = "1fffffffe"; // ffffffff * 2 = 1fffffffe in hex
        BigNum v("ffffffff");
        BigNum w("2");
        BigNum x = v * w;
        std::string result = x.toString();
        if (expected == result) {
            //std::cout << "BigNum 32-bit multiplication passed" << std::endl;
        }
        else {
            std::cout << "BigNum 32-bit multiplication failed" << std::endl;
            std::cout << "Expected: " << expected << " Result: " << result << std::endl;
        }
    }

    // Test 32-bit shift left
    {
        std::string expected = "7fffffff8"; // ffffffff << 3 = 7fffffff8 in hex
        BigNum y("ffffffff");
        BigNum a = y << 3;
        std::string result = a.toString();
        if (expected == result) {
            //std::cout << "BigNum 32-bit shift left passed" << std::endl;
        }
        else {
            std::cout << "BigNum 32-bit shift left failed" << std::endl;
			std::cout << "Expected: " << expected << " Result: " << result << std::endl;
        }
    }

    // Test 32-bit shift right
    {
        std::string expected = "1fffffff"; // ffffffff >> 3 = 1fffffff in hex
        BigNum b("ffffffff");
        BigNum c = b >> 3;
        std::string result = c.toString();
        if (expected == result) {
            //std::cout << "BigNum 32-bit shift right passed" << std::endl;
        }
        else {
            std::cout << "BigNum 32-bit shift right failed" << std::endl;
        }
    }

    

    // Test 64-bit addition
    {
        std::string expected = "10000000000000000"; // ffffffffffffffff + 1 = 10000000000000000 in hex
        BigNum e1("ffffffffffffffff");
        BigNum f1("1");
        BigNum g1 = e1 + f1;
        std::string result = g1.toString();
        if (expected == result) {
            //std::cout << "BigNum 64-bit addition passed" << std::endl;
        }
        else {
            std::cout << "BigNum 64-bit addition failed" << std::endl;
        }
    }

    // Test 64-bit subtraction
    {
        std::string expected = "fffffffffffffffe"; // ffffffffffffffff - 1 = fffffffffffffffe in hex
        BigNum h1("ffffffffffffffff");
        BigNum i1("1");
        BigNum j1 = h1 - i1;
        std::string result = j1.toString();
        if (expected == result) {
            //std::cout << "BigNum 64-bit subtraction passed" << std::endl;
        }
        else {
            std::cout << "BigNum 64-bit subtraction failed" << std::endl;
            std::cout << "Expected: " << expected << " Result: " << result << std::endl;
        }
    }

    // Test 64-bit multiplication
    {
        std::string expected = "1fffffffffffffffe"; // ffffffffffffffff * 2 = 1fffffffffffffffe in hex
        BigNum k1("ffffffffffffffff");
        BigNum l1("2");
        BigNum m1 = k1 * l1;
        std::string result = m1.toString();
        if (expected == result) {
            //std::cout << "BigNum 64-bit multiplication passed" << std::endl;
        }
        else {
            std::cout << "BigNum 64-bit multiplication failed" << std::endl;
        }
    }

    // Test 64-bit shift left
    {
        std::string expected = "7fffffffffffffff8"; // ffffffffffffffff << 3 = 7fffffffffffffff8 in hex
        BigNum n("ffffffffffffffff");
        BigNum o = n << 3;
        std::string result = o.toString();
        if (expected == result) {
            //std::cout << "BigNum 64-bit shift left passed" << std::endl;
        }
        else {
            std::cout << "BigNum 64-bit shift left failed" << std::endl;
            std::cout << "Expected: " << expected << " Result: " << result << std::endl;
        }
    }

    // Test 64-bit shift right
    {
        std::string expected = "1fffffffffffffff"; // ffffffffffffffff >> 3 = 1fffffffffffffff in hex
        BigNum p("ffffffffffffffff");
        BigNum q = p >> 3;
        std::string result = q.toString();
        if (expected == result) {
            //std::cout << "BigNum 64-bit shift right passed" << std::endl;
        }
        else {
            std::cout << "BigNum 64-bit shift right failed" << std::endl;
            std::cout << "Expected: " << expected << " Result: " << result << std::endl;
        }
    }

    // Test 64-bit division
    {
        std::string expected = "7fffffffffffffff"; // ffffffffffffffff / 2 = 7fffffffffffffff in hex
        BigNum n1("ffffffffffffffff");
        BigNum o1("2");
        BigNum p1 = n1 / o1;
        std::string result = p1.toString();
        if (expected == result) {
            //std::cout << "BigNum 64-bit division passed" << std::endl;
        }
        else {
            std::cout << "BigNum 64-bit division failed" << std::endl;
        }
    }

    // Test 64-bit modulus
    {
        std::string expected = "1"; // ffffffffffffffff % 2 = 1 in hex
        BigNum q1("ffffffffffffffff");
        BigNum r1("2");
        BigNum s1 = q1 % r1;
        std::string result = s1.toString();
        if (expected == result) {
            //std::cout << "BigNum 64-bit modulus passed" << std::endl;
        }
        else {
            std::cout << "BigNum 64-bit modulus failed" << std::endl;
        }
    }

    // Test 32-bit division
    {
        std::string expected = "7fffffff"; // ffffffff / 2 = 7fffffff in hex
        BigNum y("fffffffe");
        BigNum z("2");
        BigNum a1 = y / z;
        std::string result = a1.toString();
        if (expected == result) {
            //std::cout << "BigNum 32-bit division passed" << std::endl;
        }
        else {
            std::cout << "BigNum 32-bit division failed" << std::endl;
        }
    }

    // Test 32-bit modulus
    {
        std::string expected = "0"; // ffffffff % 2 = 0 in hex
        BigNum b1("fffffffe");
        BigNum c1("2");
        BigNum d1 = b1 % c1;
        std::string result = d1.toString();
        if (expected == result) {
            //std::cout << "BigNum 32-bit modulus passed" << std::endl;
        }
        else {
            std::cout << "BigNum 32-bit modulus failed" << std::endl;
        }
    }
    
    // Test 128-bit addition
    {
        std::string expected = "100000000000000000000000000000000"; // ffffffffffffffffffffffffffffffff + 1 = 100000000000000000000000000000000 in hex
        BigNum t1("ffffffffffffffffffffffffffffffff");
        BigNum u1("1");
        BigNum v1 = t1 + u1;
        std::string result = v1.toString();
        if (expected == result) {
            //std::cout << "BigNum 128-bit addition passed" << std::endl;
        }
        else {
            std::cout << "BigNum 128-bit addition failed" << std::endl;
        }
    }

    // Test 128-bit subtraction
    {
        std::string expected = "fffffffffffffffffffffffffffffffe"; // ffffffffffffffffffffffffffffffff - 1 = fffffffffffffffffffffffffffffffe in hex
        BigNum w1("ffffffffffffffffffffffffffffffff");
        BigNum x1("1");
        BigNum y1 = w1 - x1;
        std::string result = y1.toString();
        if (expected == result) {
            //std::cout << "BigNum 128-bit subtraction passed" << std::endl;
        }
        else {
            std::cout << "BigNum 128-bit subtraction failed" << std::endl;
            std::cout << "Expected: " << expected << " Result: " << result << std::endl;
        }
    }

    // Test 128-bit multiplication
    {
        std::string expected = "1fffffffffffffffffffffffffffffffe"; // ffffffffffffffffffffffffffffffff * 2 = 1fffffffffffffffffffffffffffffffe in hex
        BigNum z1("ffffffffffffffffffffffffffffffff");
        BigNum a2("2");
        BigNum b2 = z1 * a2;
        std::string result = b2.toString();
        if (expected == result) {
            //std::cout << "BigNum 128-bit multiplication passed" << std::endl;
        }
        else {
            std::cout << "BigNum 128-bit multiplication failed" << std::endl;
        }
    }
    // Test 128-bit shift left
    {
        std::string expected = "7fffffffffffffffffffffffffffffff8"; // ffffffffffffffffffffffffffffffff << 3 = 7fffffffffffffffffffffffffffffff8 in hex
        BigNum c1("ffffffffffffffffffffffffffffffff");
        BigNum d1 = c1 << 3;
        std::string result = d1.toString();
        if (expected == result) {
            //std::cout << "BigNum 128-bit shift left passed" << std::endl;
        }
        else {
            std::cout << "BigNum 128-bit shift left failed" << std::endl;
            std::cout << "Expected: " << expected << " Result: " << result << std::endl;
        }
    }

    // Test 128-bit shift right
    {
        std::string expected = "1fffffffffffffffffffffffffffffff"; // ffffffffffffffffffffffffffffffff >> 3 = 1fffffffffffffffffffffffffffffff in hex
        BigNum e1("ffffffffffffffffffffffffffffffff");
        BigNum f1 = e1 >> 3;
        std::string result = f1.toString();
        if (expected == result) {
            //std::cout << "BigNum 128-bit shift right passed" << std::endl;
        }
        else {
            std::cout << "BigNum 128-bit shift right failed" << std::endl;
            std::cout << "Expected: " << expected << " Result: " << result << std::endl;
        }
    }

    // Test 128-bit division
    {
        std::string expected = "7fffffffffffffffffffffffffffffff"; // ffffffffffffffffffffffffffffffff / 2 = 7fffffffffffffffffffffffffffffff in hex
        BigNum c2("ffffffffffffffffffffffffffffffff");
        BigNum d2("2");
        BigNum e2 = c2 / d2;
        std::string result = e2.toString();
        if (expected == result) {
            //std::cout << "BigNum 128-bit division passed" << std::endl;
        }
        else {
            std::cout << "BigNum 128-bit division failed" << std::endl;
        }
    }

    // Test 128-bit modulus
    {
        std::string expected = "1"; // ffffffffffffffffffffffffffffffff % 2 = 1 in hex
        BigNum f2("ffffffffffffffffffffffffffffffff");
        BigNum g2("2");
        BigNum h2 = f2 % g2;
        std::string result = h2.toString();
        if (expected == result) {
            //std::cout << "BigNum 128-bit modulus passed" << std::endl;
        }
        else {
            std::cout << "BigNum 128-bit modulus failed" << std::endl;
			std::cout << "Expected: " << expected << " Result: " << result << std::endl;
        }
    }
    


    
    // Test 128-bit < operator
    {
        BigNum i1("fffffffffffffffffffffffffffffff1");
        BigNum j2("ffffffffffffffffffffffffffffffff");


        if (i1 < j2) {
            //std::cout << "BigNum 128-bit < operator passed" << std::endl;
        }
        else {
            std::cout << "BigNum 128-bit < operator failed" << std::endl;
        }
    }

    // Test 128-bit >= operator
    {
        BigNum k1("ffffffffffffffffffffffffffffffff");
        BigNum l2("fffffffffffff1ffffffffffffffffff");

        if (k1 >= l2) {
            //std::cout << "BigNum 128-bit >= operator passed" << std::endl;
        }
        else {
            std::cout << "BigNum 128-bit >= operator failed" << std::endl;
        }
    }


    std::cout << "Testing BigNum that allrady tested done" << std::endl;




}





void testRSAWithLongInt() {
	std::cout << "Testing RSA with long int" << std::endl;
    unsigned long long int publicKey, privateKey, modulus;

    RSA::generate_keys(publicKey, privateKey, modulus, 32);

    
    // test RSA encryption and decryption for int
    unsigned long long int message = 12345;
    unsigned long long int encrypted = RSA::encrypt(message, publicKey, modulus);
    unsigned long long int decrypted = RSA::decrypt(encrypted, privateKey, modulus);

    if (message == decrypted && encrypted != message)
	{
		// std::cout << "RSA encryption and decryption for int passed" << std::endl;
	}
	else
    {
		std::cout << "RSA encryption and decryption for int failed" << std::endl;
    }


    // test RSA encryption and decryption for string
    std::string text = "Hello, RSA!";
    std::string encrypted_text = RSA::encrypt_string(text, publicKey, modulus);
    std::string decrypted_text = RSA::decrypt_string(encrypted_text, privateKey, modulus);

	if (text == decrypted_text && encrypted_text != text)
	{
		// std::cout << "RSA encryption and decryption for string passed" << std::endl;
       
       
	}
	else
	{
		std::cout << "RSA encryption and decryption for string failed" << std::endl;
        
	}


	std::cout << "Testing RSA done" << std::endl;
}




