#include <gtest/gtest.h>
#include "FACING.H"

TEST(FacingTest, NorthIsZero) {
    EXPECT_EQ(static_cast<int>(FACING_N), 0);
}