#include <gtest/gtest.h>

TEST(BigNum, eq_1_to_099999999999) {
    EXPECT_EQ(1, 0.99999999999999999999);
}

TEST(BigNum, eq_0_to_minus_0) {
    EXPECT_EQ(0, -0);
}
