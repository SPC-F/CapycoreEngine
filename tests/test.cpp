#include <gtest/gtest.h>

// A simple example function from your project
TEST(ExampleTest, CanAddNumbers) {
    auto sum = 2 + 3;
    EXPECT_EQ(sum, 5);
}