#include "lib.hpp"
#include <gtest/gtest.h>

TEST(ExampleTest, AddTest)
{
    YAQ::Example example;
    EXPECT_EQ(example.add(2, 3), 5);
    EXPECT_EQ(example.add(-1, 1), 0);
    EXPECT_EQ(example.add(0, 0), 0);
}