#include <gtest/gtest.h>
#include "BigNum.h"

//Test addition with int
TEST(BigNumTest, addition_with_int){
    BigNum a("1");
    BigNum b("f");
    BigNum c = a + b;
    std::string expected = "10";
    std::string result = c.toString();
    EXPECT_EQ(expected, result) << "BigNum addition with int failed";
}

//Test subtraction with int
TEST(BigNumTest, subtraction_with_int){
    BigNum a("10");
    BigNum b("1");
    BigNum c = a - b;
    std::string expected = "f";
    std::string result = c.toString();
    EXPECT_EQ(expected, result) << "BigNum subtraction with int failed";
}

//Test division with int
TEST(BigNumTest, division_with_int){
    BigNum a("e");
    BigNum c = a / 4;
    std::string expected = "3";
    std::string result = c.toString();
    EXPECT_EQ(expected, result) << "BigNum division with int failed";
}


//Test modulus with int
TEST(BigNumTest, modulus_with_int){
    BigNum a("b");
    BigNum c = a % 2;
    std::string expected = "1";
    std::string result = c.toString();
    EXPECT_EQ(expected, result) << "BigNum modulus with int failed";
}

//Test toString
TEST(BigNumTest, toString){
    BigNum a("f15e7");
    std::string expected = "f15e7";
    std::string result = a.toString();
    EXPECT_EQ(expected, result) << "BigNum toString failed";
}

// Test plain addition
TEST(BigNumTest, plain_addition)
{
    BigNum a("1");
    BigNum b("f");
    BigNum c = a + b;
    std::string expected = "10"; // 1 + 15 = 16 in decimal, which is 10 in hex
    std::string result = c.toString();
    EXPECT_EQ(expected, result) << "BigNum plain addition failed" ;
}

// Test plain subtraction
TEST(BigNumTest, plain_subtraction)
{
    std::string expected = "f"; // 16 - 1 = 15 in decimal, which is f in hex
    BigNum d("10");
    BigNum e("1");
    BigNum f = d - e;
    std::string result = f.toString();
    EXPECT_EQ(expected, result) << "BigNum plain subtraction failed";
}

// Test plain multiplication
TEST(BigNumTest, plain_multiplication)
{
    std::string expected = "f"; // 5 * 3 = 15 in decimal, which is f in hex
    BigNum g("5");
    BigNum h("3");
    BigNum i = g * h;
    std::string result = i.toString();
    EXPECT_EQ(expected, result) << "BigNum plain multiplication failed" ;
}

// test plain shift left
TEST(BigNumTest, plain_shift_left)
{
    std::string expected = "78"; //1111 to 111 1000 is 78 in hex
    BigNum g("f");
    BigNum i = g << 3;
    std::string result = i.toString();
    EXPECT_EQ(expected, result) << "BigNum plain shift left failed";
}

// test plain shift right
TEST(BigNumTest, plain_shift_right)
{
    std::string expected = "1"; //1111 to 0001 is 1 in hex
    BigNum g("f");
    BigNum i = g >> 3;
    std::string result = i.toString();
    EXPECT_EQ(expected, result) << "BigNum plain shift right failed";
}


// Test plain division
TEST(BigNumTest, plain_division)
{
    std::string expected = "4"; // 14 / 3 = 4 in decimal, which is 4 in hex
    BigNum j("e");
    BigNum k("3");
    BigNum l = j / k;
    std::string result = l.toString();
    EXPECT_EQ(expected, result) << "BigNum plain division failed";
}

//Test plain modulus
TEST(BigNumTest, plain_modulus)
{
    std::string expected = "2"; // 14 % 3 = 2 in decimal, which is 2 in hex
    BigNum m("e");
    BigNum n("3");
    BigNum o = m % n;
    std::string result = o.toString();
    EXPECT_EQ(expected, result) << "BigNum plain modulus failed";
}

// Test  < operator with different sizes
TEST(BigNumTest, less_operator_with_different_sizes)
{
    BigNum i1("f");
    BigNum j2("fffffffffffff");

    EXPECT_LT(i1, j2) << "BigNum different sizes < operator failed";
}

// Test  >= operator with different sizes
TEST(BigNumTest, greater_equal_operator_with_different_sizes)

{
    BigNum k1("ffffffffffffffffff");
    BigNum l2("fff");
    EXPECT_GE(k1, l2) << "BigNum different sizes >= operator passed";
}

// Test 32-bit addition
TEST(BigNumTest, addition_32_bit)
{
    std::string expected = "100000000"; // ffffffff + 1 = 100000000 in hex
    BigNum p("ffffffff");
    BigNum q("1");
    BigNum r = p + q;
    std::string result = r.toString();
    EXPECT_EQ(expected, result) << "BigNum 32-bit addition failed";
}

// Test 32-bit subtraction
TEST(BigNumTest, subtraction_32_bit)
{
    std::string expected = "fffffffe"; // ffffffff - 1 = fffffffe in hex
    BigNum s("ffffffff");
    BigNum t("1");
    BigNum u = s - t;
    std::string result = u.toString();
    EXPECT_EQ(expected, result) << "BigNum 32-bit subtraction passed";
}

// Test 32-bit multiplication
TEST(BigNumTest, multiplication_32_bit)
{
    std::string expected = "1fffffffe"; // ffffffff * 2 = 1fffffffe in hex
    BigNum v("ffffffff");
    BigNum w("2");
    BigNum x = v * w;
    std::string result = x.toString();
    EXPECT_EQ(expected, result) << "BigNum 32-bit multiplication failed";
}

// Test 32-bit shift left
TEST(BigNumTest, shift_left_32_bit)
{
    std::string expected = "7fffffff8"; // ffffffff << 3 = 7fffffff8 in hex
    BigNum y("ffffffff");
    BigNum a = y << 3;
    std::string result = a.toString();
    EXPECT_EQ(expected, result) << "BigNum 32-bit shift left failed";
}

// Test 32-bit shift right
TEST(BigNumTest, shift_right_32_bit)    
{
    std::string expected = "1fffffff"; // ffffffff >> 3 = 1fffffff in hex
    BigNum b("ffffffff");
    BigNum c = b >> 3;
    std::string result = c.toString();
    EXPECT_EQ(expected, result) << "BigNum 32-bit shift right failed";
}

// Test 64-bit addition
TEST(BigNumTest, addition_64_bit)
{
    std::string expected = "10000000000000000"; // ffffffffffffffff + 1 = 10000000000000000 in hex
    BigNum e1("ffffffffffffffff");
    BigNum f1("1");
    BigNum g1 = e1 + f1;
    std::string result = g1.toString();
    EXPECT_EQ(expected, result) << "BigNum 64-bit addition failed";
}

// Test 64-bit subtraction
TEST(BigNumTest, subtraction_64_bit)
{
    std::string expected = "fffffffffffffffe"; // ffffffffffffffff - 1 = fffffffffffffffe in hex
    BigNum h1("ffffffffffffffff");
    BigNum i1("1");
    BigNum j1 = h1 - i1;
    std::string result = j1.toString();
    EXPECT_EQ(expected, result) << "BigNum 64-bit subtraction failed";
}

// Test 64-bit multiplication
TEST(BigNumTest, multiplication_64_bit)
{
    std::string expected = "1fffffffffffffffe"; // ffffffffffffffff * 2 = 1fffffffffffffffe in hex
    BigNum k1("ffffffffffffffff");
    BigNum l1("2");
    BigNum m1 = k1 * l1;
    std::string result = m1.toString();
    EXPECT_EQ(expected, result) << "BigNum 64-bit multiplication failed";
}

// Test 64-bit shift left
TEST(BigNumTest, shift_left_64_bit)
{
    std::string expected = "7fffffffffffffff8"; // ffffffffffffffff << 3 = 7fffffffffffffff8 in hex
    BigNum n("ffffffffffffffff");
    BigNum o = n << 3;
    std::string result = o.toString();
    EXPECT_EQ(expected, result) << "BigNum 64-bit shift left failed" ;
}

// Test 64-bit shift right
TEST(BigNumTest, shift_right_64_bit)
{
    std::string expected = "1fffffffffffffff"; // ffffffffffffffff >> 3 = 1fffffffffffffff in hex
    BigNum p("ffffffffffffffff");
    BigNum q = p >> 3;
    std::string result = q.toString();
    EXPECT_EQ(expected, result) << "BigNum 64-bit shift right failed";
}

// Test 64-bit division
TEST(BigNumTest, division_64_bit)
{
    std::string expected = "7fffffffffffffff"; // ffffffffffffffff / 2 = 7fffffffffffffff in hex
    BigNum n1("ffffffffffffffff");
    BigNum o1("2");
    BigNum p1 = n1 / o1;
    std::string result = p1.toString();
    EXPECT_EQ(expected, result) << "BigNum 64-bit division failed";
}

// Test 64-bit modulus
TEST(BigNumTest, modulus_64_bit)
{
    std::string expected = "1"; // ffffffffffffffff % 2 = 1 in hex
    BigNum q1("ffffffffffffffff");
    BigNum r1("2");
    BigNum s1 = q1 % r1;
    std::string result = s1.toString();
    EXPECT_EQ(expected, result) << "BigNum 64-bit modulus failed";
}

// Test 32-bit division
TEST(BigNumTest, division_32_bit)
{
    std::string expected = "7fffffff"; // ffffffff / 2 = 7fffffff in hex
    BigNum y("fffffffe");
    BigNum z("2");
    BigNum a1 = y / z;
    std::string result = a1.toString();
    EXPECT_EQ(expected, result) << "BigNum 32-bit division failed";
}

// Test 32-bit modulus
TEST(BigNumTest, modulus_32_bit)
{
    std::string expected = "0"; // ffffffff % 2 = 0 in hex
    BigNum b1("fffffffe");
    BigNum c1("2");
    BigNum d1 = b1 % c1;
    std::string result = d1.toString();
    EXPECT_EQ(expected, result) << "BigNum 32-bit modulus failed" ;
}

// Test 128-bit addition
TEST(BigNumTest, addition_128_bit)
{
    std::string expected = "100000000000000000000000000000000"; // ffffffffffffffffffffffffffffffff + 1 = 100000000000000000000000000000000 in hex
    BigNum t1("ffffffffffffffffffffffffffffffff");
    BigNum u1("1");
    BigNum v1 = t1 + u1;
    std::string result = v1.toString();
    EXPECT_EQ(expected, result) << "BigNum 128-bit addition failed" ;
}

// Test 128-bit subtraction
TEST(BigNumTest, subtraction_128_bit)
{
    std::string expected = "fffffffffffffffffffffffffffffffe"; // ffffffffffffffffffffffffffffffff - 1 = fffffffffffffffffffffffffffffffe in hex
    BigNum w1("ffffffffffffffffffffffffffffffff");
    BigNum x1("1");
    BigNum y1 = w1 - x1;
    std::string result = y1.toString();
    EXPECT_EQ(expected, result) << "BigNum 128-bit subtraction failed";
}

// Test 128-bit multiplication
TEST(BigNumTest, multiplication_128_bit)
{
    std::string expected = "1fffffffffffffffffffffffffffffffe"; // ffffffffffffffffffffffffffffffff * 2 = 1fffffffffffffffffffffffffffffffe in hex
    BigNum z1("ffffffffffffffffffffffffffffffff");
    BigNum a2("2");
    BigNum b2 = z1 * a2;
    std::string result = b2.toString();
    EXPECT_EQ(expected, result) << "BigNum 128-bit multiplication failed";
}

// Test 128-bit shift left
TEST(BigNumTest, shift_left_128_bit)
{
    std::string expected = "7fffffffffffffffffffffffffffffff8"; // ffffffffffffffffffffffffffffffff << 3 = 7fffffffffffffffffffffffffffffff8 in hex
    BigNum c1("ffffffffffffffffffffffffffffffff");
    BigNum d1 = c1 << 3;
    std::string result = d1.toString();
    EXPECT_EQ(expected, result) << "BigNum 128-bit shift left failed";
}

// Test 128-bit shift right
TEST(BigNumTest, shift_right_128_bit)
{
    std::string expected = "1fffffffffffffffffffffffffffffff"; // ffffffffffffffffffffffffffffffff >> 3 = 1fffffffffffffffffffffffffffffff in hex
    BigNum e1("ffffffffffffffffffffffffffffffff");
    BigNum f1 = e1 >> 3;
    std::string result = f1.toString();
    EXPECT_EQ(expected, result) << "BigNum 128-bit shift right failed";
}

// Test 128-bit division
TEST(BigNumTest, division_128_bit)
{
    std::string expected = "7fffffffffffffffffffffffffffffff"; // ffffffffffffffffffffffffffffffff / 2 = 7fffffffffffffffffffffffffffffff in hex
    BigNum c2("ffffffffffffffffffffffffffffffff");
    BigNum d2("2");
    BigNum e2 = c2 / d2;
    std::string result = e2.toString();
    EXPECT_EQ(expected, result) << "BigNum 128-bit division failed";
}

// Test 128-bit modulus
TEST(BigNumTest, modulus_128_bit)
{
    std::string expected = "1"; // ffffffffffffffffffffffffffffffff % 2 = 1 in hex
    BigNum f2("ffffffffffffffffffffffffffffffff");
    BigNum g2("2");
    BigNum h2 = f2 % g2;
    std::string result = h2.toString();
    EXPECT_EQ(expected, result) << "BigNum 128-bit modulus failed";
}

// Test 128-bit < operator
TEST(BigNumTest,  less_operator_128_bit)
{
    BigNum i1("fffffffffffffffffffffffffffffff1");
    BigNum j2("ffffffffffffffffffffffffffffffff");

    EXPECT_LT(i1, j2) << "BigNum 128-bit < operator failed";
}

// Test 128-bit >= operator
TEST(BigNumTest,   greater_equal_operator_128_bit)
{
    BigNum k1("ffffffffffffffffffffffffffffffff");
    BigNum l2("fffffffffffff1ffffffffffffffffff");
    EXPECT_GE(k1, l2) << "BigNum 128-bit >= operator failed";
}




