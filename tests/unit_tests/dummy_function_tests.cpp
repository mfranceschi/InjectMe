#include <limits>
#include <stdexcept>
#include <gtest/gtest.h>
#include "InjectMe.hpp"

TEST(dummy_function, itReturnsTheSum)
{
    ASSERT_EQ(mf::InjectMe::addIntegersOrThrow(50, 7), 57);
    ASSERT_EQ(mf::InjectMe::addIntegersOrThrow(-50, 7), -43);
    ASSERT_EQ(mf::InjectMe::addIntegersOrThrow(50, -7), 43);
    ASSERT_EQ(mf::InjectMe::addIntegersOrThrow(-50, -7), -57);
}

TEST(dummy_function, itThrowsIfItWillOverflow)
{
    ASSERT_THROW(mf::InjectMe::addIntegersOrThrow(std::numeric_limits<int>::max(), 1), std::overflow_error);
}

TEST(dummy_function, itThrowsIfItWillUnderflow)
{
    ASSERT_THROW(mf::InjectMe::addIntegersOrThrow(std::numeric_limits<int>::min(), -1), std::underflow_error);
}
