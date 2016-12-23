#include <gtest/gtest.h>
#include <implicit_future>

TEST(ImplicitFutureTestSuite, ObviousImplicitCast)
{
    std::future<int> test = std::async([](){return 42;});
    implicit_future<int> implicit_test{std::move(test)};
    int implicitly_casted = implicit_test;
    ASSERT_EQ(implicitly_casted, 42);
}

