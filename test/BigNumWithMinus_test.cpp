#include <gtest/gtest.h>
#include "BigNumWithMinus.h"

//Test addition with int
TEST(BigNumWithMinusTest, addition_with_int){
    BigNumWithMinus a("1");
    BigNumWithMinus b("f");
    BigNumWithMinus c = a + b;
    std::string expected = "10";
    std::string result = c.toString();
    EXPECT_EQ(expected, result) << "BigNumWithMinus addition with int failed";
}

//Test subtraction with int
TEST(BigNumWithMinusTest, subtraction_with_int){
    BigNumWithMinus a("10");
    BigNumWithMinus b("1");
    BigNumWithMinus c = a - b;
    std::string expected = "f";
    std::string result = c.toString();
    EXPECT_EQ(expected, result) << "BigNumWithMinus subtraction with int failed";
}

//Test division with int
TEST(BigNumWithMinusTest, division_with_int){
    BigNumWithMinus a("e");
    BigNumWithMinus c = a / 4;
    std::string expected = "3";
    std::string result = c.toString();
    EXPECT_EQ(expected, result) << "BigNumWithMinus division with int failed" << "expected: " << expected << " result: " << result;
    
}


//Test modulus with int
TEST(BigNumWithMinusTest, modulus_with_int){
    BigNumWithMinus a("b");
    BigNumWithMinus c = a % 2;
    std::string expected = "1";
    std::string result = c.toString();
    EXPECT_EQ(expected, result) << "BigNumWithMinus modulus with int failed";
}

//Test toString
TEST(BigNumWithMinusTest, toString){
    BigNumWithMinus a("f15e7");
    std::string expected = "f15e7";
    std::string result = a.toString();
    EXPECT_EQ(expected, result) << "BigNumWithMinus toString failed";
}

// Test plain addition
TEST(BigNumWithMinusTest, plain_addition)
{
    BigNumWithMinus a("1");
    BigNumWithMinus b("f");
    BigNumWithMinus c = a + b;
    std::string expected = "10"; // 1 + 15 = 16 in decimal, which is 10 in hex
    std::string result = c.toString();
    EXPECT_EQ(expected, result) << "BigNumWithMinus plain addition failed" ;
}

// Test plain subtraction
TEST(BigNumWithMinusTest, plain_subtraction)
{
    std::string expected = "f"; // 16 - 1 = 15 in decimal, which is f in hex
    BigNumWithMinus d("10");
    BigNumWithMinus e("1");
    BigNumWithMinus f = d - e;
    std::string result = f.toString();
    EXPECT_EQ(expected, result) << "BigNumWithMinus plain subtraction failed";
}

// Test plain multiplication
TEST(BigNumWithMinusTest, plain_multiplication)
{
    std::string expected = "f"; // 5 * 3 = 15 in decimal, which is f in hex
    BigNumWithMinus g("5");
    BigNumWithMinus h("3");
    BigNumWithMinus i = g * h;
    std::string result = i.toString();
    EXPECT_EQ(expected, result) << "BigNumWithMinus plain multiplication failed" ;
}

// test plain shift left
TEST(BigNumWithMinusTest, plain_shift_left)
{
    std::string expected = "78"; //1111 to 111 1000 is 78 in hex
    BigNumWithMinus g("f");
    BigNumWithMinus i = g << 3;
    std::string result = i.toString();
    EXPECT_EQ(expected, result) << "BigNumWithMinus plain shift left failed";
}

// test plain shift right
TEST(BigNumWithMinusTest, plain_shift_right)
{
    std::string expected = "1"; //1111 to 0001 is 1 in hex
    BigNumWithMinus g("f");
    BigNumWithMinus i = g >> 3;
    std::string result = i.toString();
    EXPECT_EQ(expected, result) << "BigNumWithMinus plain shift right failed";
}


// Test plain division
TEST(BigNumWithMinusTest, plain_division)
{
    std::string expected = "4"; // 14 / 3 = 4 in decimal, which is 4 in hex
    BigNumWithMinus j("e");
    BigNumWithMinus k("3");
    BigNumWithMinus l = j / k;
    std::string result = l.toString();
    EXPECT_EQ(expected, result) << "BigNumWithMinus plain division failed";
}

//Test plain modulus
TEST(BigNumWithMinusTest, plain_modulus)
{
    std::string expected = "2"; // 14 % 3 = 2 in decimal, which is 2 in hex
    BigNumWithMinus m("e");
    BigNumWithMinus n("3");
    BigNumWithMinus o = m % n;
    std::string result = o.toString();
    EXPECT_EQ(expected, result) << "BigNumWithMinus plain modulus failed";
}

// Test  < operator with different sizes
TEST(BigNumWithMinusTest, less_operator_with_different_sizes)
{
    BigNumWithMinus i1("f");
    BigNumWithMinus j2("fffffffffffff");

    EXPECT_LT(i1, j2) << "BigNumWithMinus different sizes < operator failed";
}

// Test  >= operator with different sizes
TEST(BigNumWithMinusTest, greater_equal_operator_with_different_sizes)

{
    BigNumWithMinus k1("ffffffffffffffffff");
    BigNumWithMinus l2("fff");
    EXPECT_GE(k1, l2) << "BigNumWithMinus different sizes >= operator passed";
}

// Test 32-bit addition
TEST(BigNumWithMinusTest, addition_32_bit)
{
    std::string expected = "100000000"; // ffffffff + 1 = 100000000 in hex
    BigNumWithMinus p("ffffffff");
    BigNumWithMinus q("1");
    BigNumWithMinus r = p + q;
    std::string result = r.toString();
    EXPECT_EQ(expected, result) << "BigNumWithMinus 32-bit addition failed";
}

// Test 32-bit subtraction
TEST(BigNumWithMinusTest, subtraction_32_bit)
{
    std::string expected = "fffffffe"; // ffffffff - 1 = fffffffe in hex
    BigNumWithMinus s("ffffffff");
    BigNumWithMinus t("1");
    BigNumWithMinus u = s - t;
    std::string result = u.toString();
    EXPECT_EQ(expected, result) << "BigNumWithMinus 32-bit subtraction passed";
}

// Test 32-bit multiplication
TEST(BigNumWithMinusTest, multiplication_32_bit)
{
    std::string expected = "1fffffffe"; // ffffffff * 2 = 1fffffffe in hex
    BigNumWithMinus v("ffffffff");
    BigNumWithMinus w("2");
    BigNumWithMinus x = v * w;
    std::string result = x.toString();
    EXPECT_EQ(expected, result) << "BigNumWithMinus 32-bit multiplication failed";
}

// Test 32-bit shift left
TEST(BigNumWithMinusTest, shift_left_32_bit)
{
    std::string expected = "7fffffff8"; // ffffffff << 3 = 7fffffff8 in hex
    BigNumWithMinus y("ffffffff");
    BigNumWithMinus a = y << 3;
    std::string result = a.toString();
    EXPECT_EQ(expected, result) << "BigNumWithMinus 32-bit shift left failed";
}

// Test 32-bit shift right
TEST(BigNumWithMinusTest, shift_right_32_bit)    
{
    std::string expected = "1fffffff"; // ffffffff >> 3 = 1fffffff in hex
    BigNumWithMinus b("ffffffff");
    BigNumWithMinus c = b >> 3;
    std::string result = c.toString();
    EXPECT_EQ(expected, result) << "BigNumWithMinus 32-bit shift right failed";
}

// Test 64-bit addition
TEST(BigNumWithMinusTest, addition_64_bit)
{
    std::string expected = "10000000000000000"; // ffffffffffffffff + 1 = 10000000000000000 in hex
    BigNumWithMinus e1("ffffffffffffffff");
    BigNumWithMinus f1("1");
    BigNumWithMinus g1 = e1 + f1;
    std::string result = g1.toString();
    EXPECT_EQ(expected, result) << "BigNumWithMinus 64-bit addition failed";
}

// Test 64-bit subtraction
TEST(BigNumWithMinusTest, subtraction_64_bit)
{
    std::string expected = "fffffffffffffffe"; // ffffffffffffffff - 1 = fffffffffffffffe in hex
    BigNumWithMinus h1("ffffffffffffffff");
    BigNumWithMinus i1("1");
    BigNumWithMinus j1 = h1 - i1;
    std::string result = j1.toString();
    EXPECT_EQ(expected, result) << "BigNumWithMinus 64-bit subtraction failed";
}

// Test 64-bit multiplication
TEST(BigNumWithMinusTest, multiplication_64_bit)
{
    std::string expected = "1fffffffffffffffe"; // ffffffffffffffff * 2 = 1fffffffffffffffe in hex
    BigNumWithMinus k1("ffffffffffffffff");
    BigNumWithMinus l1("2");
    BigNumWithMinus m1 = k1 * l1;
    std::string result = m1.toString();
    EXPECT_EQ(expected, result) << "BigNumWithMinus 64-bit multiplication failed";
}

// Test 64-bit shift left
TEST(BigNumWithMinusTest, shift_left_64_bit)
{
    std::string expected = "7fffffffffffffff8"; // ffffffffffffffff << 3 = 7fffffffffffffff8 in hex
    BigNumWithMinus n("ffffffffffffffff");
    BigNumWithMinus o = n << 3;
    std::string result = o.toString();
    EXPECT_EQ(expected, result) << "BigNumWithMinus 64-bit shift left failed" ;
}

// Test 64-bit shift right
TEST(BigNumWithMinusTest, shift_right_64_bit)
{
    std::string expected = "1fffffffffffffff"; // ffffffffffffffff >> 3 = 1fffffffffffffff in hex
    BigNumWithMinus p("ffffffffffffffff");
    BigNumWithMinus q = p >> 3;
    std::string result = q.toString();
    EXPECT_EQ(expected, result) << "BigNumWithMinus 64-bit shift right failed";
}

// Test 64-bit division
TEST(BigNumWithMinusTest, division_64_bit)
{
    std::string expected = "7fffffffffffffff"; // ffffffffffffffff / 2 = 7fffffffffffffff in hex
    BigNumWithMinus n1("ffffffffffffffff");
    BigNumWithMinus o1("2");
    BigNumWithMinus p1 = n1 / o1;
    std::string result = p1.toString();
    EXPECT_EQ(expected, result) << "BigNumWithMinus 64-bit division failed";
}

// Test 64-bit modulus
TEST(BigNumWithMinusTest, modulus_64_bit)
{
    std::string expected = "1"; // ffffffffffffffff % 2 = 1 in hex
    BigNumWithMinus q1("ffffffffffffffff");
    BigNumWithMinus r1("2");
    BigNumWithMinus s1 = q1 % r1;
    std::string result = s1.toString();
    EXPECT_EQ(expected, result) << "BigNumWithMinus 64-bit modulus failed";
}

// Test 32-bit division
TEST(BigNumWithMinusTest, division_32_bit)
{
    std::string expected = "7fffffff"; // ffffffff / 2 = 7fffffff in hex
    BigNumWithMinus y("fffffffe");
    BigNumWithMinus z("2");
    BigNumWithMinus a1 = y / z;
    std::string result = a1.toString();
    EXPECT_EQ(expected, result) << "BigNumWithMinus 32-bit division failed";
}

// Test 32-bit modulus
TEST(BigNumWithMinusTest, modulus_32_bit)
{
    std::string expected = "0"; // ffffffff % 2 = 0 in hex
    BigNumWithMinus b1("fffffffe");
    BigNumWithMinus c1("2");
    BigNumWithMinus d1 = b1 % c1;
    std::string result = d1.toString();
    EXPECT_EQ(expected, result) << "BigNumWithMinus 32-bit modulus failed" ;
}

// Test 128-bit addition
TEST(BigNumWithMinusTest, addition_128_bit)
{
    std::string expected = "100000000000000000000000000000000"; // ffffffffffffffffffffffffffffffff + 1 = 100000000000000000000000000000000 in hex
    BigNumWithMinus t1("ffffffffffffffffffffffffffffffff");
    BigNumWithMinus u1("1");
    BigNumWithMinus v1 = t1 + u1;
    std::string result = v1.toString();
    EXPECT_EQ(expected, result) << "BigNumWithMinus 128-bit addition failed" ;
}

// Test 128-bit subtraction
TEST(BigNumWithMinusTest, subtraction_128_bit)
{
    std::string expected = "fffffffffffffffffffffffffffffffe"; // ffffffffffffffffffffffffffffffff - 1 = fffffffffffffffffffffffffffffffe in hex
    BigNumWithMinus w1("ffffffffffffffffffffffffffffffff");
    BigNumWithMinus x1("1");
    BigNumWithMinus y1 = w1 - x1;
    std::string result = y1.toString();
    EXPECT_EQ(expected, result) << "BigNumWithMinus 128-bit subtraction failed";
}

// Test 128-bit multiplication
TEST(BigNumWithMinusTest, multiplication_128_bit)
{
    std::string expected = "1fffffffffffffffffffffffffffffffe"; // ffffffffffffffffffffffffffffffff * 2 = 1fffffffffffffffffffffffffffffffe in hex
    BigNumWithMinus z1("ffffffffffffffffffffffffffffffff");
    BigNumWithMinus a2("2");
    BigNumWithMinus b2 = z1 * a2;
    std::string result = b2.toString();
    EXPECT_EQ(expected, result) << "BigNumWithMinus 128-bit multiplication failed";
}

// Test 128-bit shift left
TEST(BigNumWithMinusTest, shift_left_128_bit)
{
    std::string expected = "7fffffffffffffffffffffffffffffff8"; // ffffffffffffffffffffffffffffffff << 3 = 7fffffffffffffffffffffffffffffff8 in hex
    BigNumWithMinus c1("ffffffffffffffffffffffffffffffff");
    BigNumWithMinus d1 = c1 << 3;
    std::string result = d1.toString();
    EXPECT_EQ(expected, result) << "BigNumWithMinus 128-bit shift left failed";
}

// Test 128-bit shift right
TEST(BigNumWithMinusTest, shift_right_128_bit)
{
    std::string expected = "1fffffffffffffffffffffffffffffff"; // ffffffffffffffffffffffffffffffff >> 3 = 1fffffffffffffffffffffffffffffff in hex
    BigNumWithMinus e1("ffffffffffffffffffffffffffffffff");
    BigNumWithMinus f1 = e1 >> 3;
    std::string result = f1.toString();
    EXPECT_EQ(expected, result) << "BigNumWithMinus 128-bit shift right failed";
}

// Test 128-bit division
TEST(BigNumWithMinusTest, division_128_bit)
{
    std::string expected = "7fffffffffffffffffffffffffffffff"; // ffffffffffffffffffffffffffffffff / 2 = 7fffffffffffffffffffffffffffffff in hex
    BigNumWithMinus c2("ffffffffffffffffffffffffffffffff");
    BigNumWithMinus d2("2");
    BigNumWithMinus e2 = c2 / d2;
    std::string result = e2.toString();
    EXPECT_EQ(expected, result) << "BigNumWithMinus 128-bit division failed";
}

// Test 128-bit modulus
TEST(BigNumWithMinusTest, modulus_128_bit)
{
    std::string expected = "1"; // ffffffffffffffffffffffffffffffff % 2 = 1 in hex
    BigNumWithMinus f2("ffffffffffffffffffffffffffffffff");
    BigNumWithMinus g2("2");
    BigNumWithMinus h2 = f2 % g2;
    std::string result = h2.toString();
    EXPECT_EQ(expected, result) << "BigNumWithMinus 128-bit modulus failed";
}

// Test 128-bit < operator
TEST(BigNumWithMinusTest,  less_operator_128_bit)
{
    BigNumWithMinus i1("fffffffffffffffffffffffffffffff1");
    BigNumWithMinus j2("ffffffffffffffffffffffffffffffff");

    EXPECT_LT(i1, j2) << "BigNumWithMinus 128-bit < operator failed";
}

// Test 128-bit >= operator
TEST(BigNumWithMinusTest,   greater_equal_operator_128_bit)
{
    BigNumWithMinus k1("ffffffffffffffffffffffffffffffff");
    BigNumWithMinus l2("fffffffffffff1ffffffffffffffffff");
    EXPECT_GE(k1, l2) << "BigNumWithMinus 128-bit >= operator failed";
}




// Test addition with int
TEST(BigNumWithMinusTest, AdditionWithInt) {
    BigNumWithMinus a("1");
    BigNumWithMinus b("f");
    BigNumWithMinus c = a + b;
    std::string expected = "10";
    std::string result = c.toString();
    EXPECT_EQ(expected, result) << "BigNumWithMinus addition with int failed";
}

// Test equality operator
TEST(BigNumWithMinusTest, EqualityOperator) {
    BigNumWithMinus a("10");
    BigNumWithMinus b("10");
    EXPECT_TRUE(a == b) << "BigNumWithMinus equality operator failed";
}

// Test inequality operator
TEST(BigNumWithMinusTest, InequalityOperator) {
    BigNumWithMinus a("10");
    BigNumWithMinus b("f");
    EXPECT_TRUE(a != b) << "BigNumWithMinus inequality operator failed";
}

// Test less than operator
TEST(BigNumWithMinusTest, LessThanOperator) {
    BigNumWithMinus a("10");
    BigNumWithMinus b("f");
    EXPECT_FALSE(a < b) << "BigNumWithMinus less than operator failed";
}

// Test greater than operator
TEST(BigNumWithMinusTest, GreaterThanOperator) {
    BigNumWithMinus a("10");
    BigNumWithMinus b("f");
    EXPECT_TRUE(a > b) << "BigNumWithMinus greater than operator failed";
}

// Test less than or equal operator
TEST(BigNumWithMinusTest, LessThanOrEqualOperator) {
    BigNumWithMinus a("10");
    BigNumWithMinus b("10");
    EXPECT_TRUE(a <= b) << "BigNumWithMinus less than or equal operator failed";
}

// Test greater than or equal operator
TEST(BigNumWithMinusTest, GreaterThanOrEqualOperator) {
    BigNumWithMinus a("10");
    BigNumWithMinus b("10");
    EXPECT_TRUE(a >= b) << "BigNumWithMinus greater than or equal operator failed";
}

// Test subtraction with positive and negative numbers
TEST(BigNumWithMinusTest, SubtractionPositiveNegativeNumbers) {
    BigNumWithMinus a("10");
    BigNumWithMinus b("-f");
    BigNumWithMinus c = a - b;
    std::string expected = "1f";
    std::string result = c.toString();
    EXPECT_EQ(expected, result) << "BigNumWithMinus subtraction with positive and negative numbers failed";
}

// Test multiplication with positive and negative numbers
TEST(BigNumWithMinusTest, MultiplicationPositiveNegativeNumbers) {
    BigNumWithMinus a("2");
    BigNumWithMinus b("-3");
    BigNumWithMinus c = a * b;
    std::string expected = "-6";
    std::string result = c.toString();
    EXPECT_EQ(expected, result) << "BigNumWithMinus multiplication with positive and negative numbers failed";
}

// Test division with positive and negative numbers
TEST(BigNumWithMinusTest, DivisionPositiveNegativeNumbers) {
    BigNumWithMinus a("6");
    BigNumWithMinus b("-3");
    BigNumWithMinus c = a / b;
    std::string expected = "-2";
    std::string result = c.toString();
    EXPECT_EQ(expected, result) << "BigNumWithMinus division with positive and negative numbers failed";
}

// Test modulus with positive and negative numbers
TEST(BigNumWithMinusTest, ModulusPositiveNegativeNumbers) {
    BigNumWithMinus a("10");
    BigNumWithMinus b("-3");
    BigNumWithMinus c = a % b;
    std::string expected = "-2";
    std::string result = c.toString();
    EXPECT_EQ(expected, result) << "BigNumWithMinus modulus with positive and negative numbers failed";
}

// Test isMinus method for negative number
TEST(BigNumWithMinusTest, IsMinusForNegativeNumber) {
    BigNumWithMinus a("-10");
    EXPECT_TRUE(a.isMinus()) << "BigNumWithMinus isMinus method failed for negative number";
}

// Test isMinus method for positive number
TEST(BigNumWithMinusTest, IsMinusForPositiveNumber) {
    BigNumWithMinus a("10");
    EXPECT_FALSE(a.isMinus()) << "BigNumWithMinus isMinus method failed for positive number";
}

// Test toString method for positive number
TEST(BigNumWithMinusTest, ToStringForPositiveNumber) {
    BigNumWithMinus a("10");
    std::string expected = "10";
    std::string result = a.toString();
    EXPECT_EQ(expected, result) << "BigNumWithMinus toString method failed for positive number";
}

// Test toString method for negative number
TEST(BigNumWithMinusTest, ToStringForNegativeNumber) {
    BigNumWithMinus a("-10");
    std::string expected = "-10";
    std::string result = a.toString();
    EXPECT_EQ(expected, result) << "BigNumWithMinus toString method failed for negative number";
}

// Test addition with large numbers
TEST(BigNumWithMinusTest, AdditionWithLargeNumbers) {
    BigNumWithMinus a("1000000000000000000000000");
    BigNumWithMinus b("1000000000000000000000000");
    BigNumWithMinus c = a + b;
    std::string expected = "2000000000000000000000000";
    std::string result = c.toString();
    EXPECT_EQ(expected, result) << "BigNumWithMinus addition with large numbers failed";
}

// Test subtraction with large numbers
TEST(BigNumWithMinusTest, SubtractionWithLargeNumbers) {
    BigNumWithMinus a("2000000000000000000000000");
    BigNumWithMinus b("1000000000000000000000000");
    BigNumWithMinus c = a - b;
    std::string expected = "1000000000000000000000000";
    std::string result = c.toString();
    EXPECT_EQ(expected, result) << "BigNumWithMinus subtraction with large numbers failed";
}

// Test multiplication with large numbers
TEST(BigNumWithMinusTest, MultiplicationWithLargeNumbers) {
    BigNumWithMinus a("1000000000000");
    BigNumWithMinus b("1000000000000");
    BigNumWithMinus c = a * b;
    std::string expected = "1000000000000000000000000";
    std::string result = c.toString();
    EXPECT_EQ(expected, result) << "BigNumWithMinus multiplication with large numbers failed";
}

// Test division with large numbers
TEST(BigNumWithMinusTest, DivisionWithLargeNumbers) {
    BigNumWithMinus a("1000000000000000000000000");
    BigNumWithMinus b("1000000000000");
    BigNumWithMinus c = a / b;
    std::string expected = "1000000000000";
    std::string result = c.toString();
    EXPECT_EQ(expected, result) << "BigNumWithMinus division with large numbers failed";
}

// Test modulus with large numbers
TEST(BigNumWithMinusTest, ModulusWithLargeNumbers) {
    BigNumWithMinus a("1000000000000000000000001");
    BigNumWithMinus b("1000000000000");
    BigNumWithMinus c = a % b;
    std::string expected = "1";
    std::string result = c.toString();
    EXPECT_EQ(expected, result) << "BigNumWithMinus modulus with large numbers failed";
}


// Test addition with positive numbers
TEST(BigNumWithMinusTest, AdditionWithPositiveNumbers) {
    BigNumWithMinus a("1");
    BigNumWithMinus b("f");
    BigNumWithMinus c = a + b;
    std::string expected = "10";
    std::string result = c.toString();
    EXPECT_EQ(expected, result) << "BigNumWithMinus addition with positive numbers failed";
}

// Test addition with negative numbers
TEST(BigNumWithMinusTest, AdditionWithNegativeNumbers) {
    BigNumWithMinus a("-1");
    BigNumWithMinus b("-f");
    BigNumWithMinus c = a + b;
    std::string expected = "-10";
    std::string result = c.toString();
    EXPECT_EQ(expected, result) << "BigNumWithMinus addition with negative numbers failed";
}

// Test addition with mixed signs
TEST(BigNumWithMinusTest, AdditionWithMixedSigns) {
    BigNumWithMinus a("-1");
    BigNumWithMinus b("f");
    BigNumWithMinus c = a + b;
    std::string expected = "e";
    std::string result = c.toString();
    EXPECT_EQ(expected, result) << "BigNumWithMinus addition with mixed signs failed";
}

// Test subtraction with positive numbers
TEST(BigNumWithMinusTest, SubtractionWithPositiveNumbers) {
    BigNumWithMinus a("10");
    BigNumWithMinus b("f");
    BigNumWithMinus c = a - b;
    std::string expected = "1";
    std::string result = c.toString();
    EXPECT_EQ(expected, result) << "BigNumWithMinus subtraction with positive numbers failed";
}

// Test subtraction with negative numbers
TEST(BigNumWithMinusTest, SubtractionWithNegativeNumbers) {
    BigNumWithMinus a("-10");
    BigNumWithMinus b("-f");
    BigNumWithMinus c = a - b;
    std::string expected = "-1";
    std::string result = c.toString();
    EXPECT_EQ(expected, result) << "BigNumWithMinus subtraction with negative numbers failed";
}

// Test multiplication with positive numbers
TEST(BigNumWithMinusTest, MultiplicationWithPositiveNumbers) {
    BigNumWithMinus a("2");
    BigNumWithMinus b("3");
    BigNumWithMinus c = a * b;
    std::string expected = "6";
    std::string result = c.toString();
    EXPECT_EQ(expected, result) << "BigNumWithMinus multiplication with positive numbers failed";
}

// Test multiplication with negative numbers
TEST(BigNumWithMinusTest, MultiplicationWithNegativeNumbers) {
    BigNumWithMinus a("-2");
    BigNumWithMinus b("-3");
    BigNumWithMinus c = a * b;
    std::string expected = "6";
    std::string result = c.toString();
    EXPECT_EQ(expected, result) << "BigNumWithMinus multiplication with negative numbers failed";
}

// Test division with positive numbers
TEST(BigNumWithMinusTest, DivisionWithPositiveNumbers) {
    BigNumWithMinus a("6");
    BigNumWithMinus b("3");
    BigNumWithMinus c = a / b;
    std::string expected = "2";
    std::string result = c.toString();
    EXPECT_EQ(expected, result) << "BigNumWithMinus division with positive numbers failed";
}

// Test division with negative numbers
TEST(BigNumWithMinusTest, DivisionWithNegativeNumbers) {
    BigNumWithMinus a("-6");
    BigNumWithMinus b("-3");
    BigNumWithMinus c = a / b;
    std::string expected = "2";
    std::string result = c.toString();
    EXPECT_EQ(expected, result) << "BigNumWithMinus division with negative numbers failed";
}

// Test modulus with positive numbers
TEST(BigNumWithMinusTest, ModulusWithPositiveNumbers) {
    BigNumWithMinus a("10");
    BigNumWithMinus b("3");
    BigNumWithMinus c = a % b;
    std::string expected = "1";
    std::string result = c.toString();
    EXPECT_EQ(expected, result) << "BigNumWithMinus modulus with positive numbers failed";
}

// Test modulus with negative numbers
TEST(BigNumWithMinusTest, ModulusWithNegativeNumbers) {
    BigNumWithMinus a("-7");
    BigNumWithMinus b("5");
    BigNumWithMinus c = a % b;
    std::string expected = "3";
    std::string result = c.toString();
    EXPECT_EQ(expected, result) << "BigNumWithMinus modulus with negative numbers failed";
}







