#include <gtest/gtest.h>

#include <limits>
#include <stdexcept>

#include "InjectMe.hpp"

using mf::InjectMe::addIntegersOrThrow;

TEST(dummy_function, itReturnsTheSum) {
  ASSERT_EQ(addIntegersOrThrow(50, 7), 57);
  ASSERT_EQ(addIntegersOrThrow(-50, 7), -43);
  ASSERT_EQ(addIntegersOrThrow(50, -7), 43);
  ASSERT_EQ(addIntegersOrThrow(-50, -7), -57);
}

TEST(dummy_function, itThrowsIfItWillOverflow) {
  ASSERT_THROW(addIntegersOrThrow(std::numeric_limits<int>::max(), 1), std::overflow_error);
}

TEST(dummy_function, itThrowsIfItWillUnderflow) {
  ASSERT_THROW(addIntegersOrThrow(std::numeric_limits<int>::min(), -1), std::underflow_error);
}
