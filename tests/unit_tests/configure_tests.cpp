#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <stdexcept>

#include "InjectMe.hpp"

using namespace mf::InjectMe;

static void validDeleter(int*) {
}

static int* validProvider() {
  return new int(1);
}

static int* failingProvider() {
  throw std::runtime_error("The provider failed successfully!");
}

class ConfigureTestsBase : public ::testing::Test {
 protected:
  void SetUp() override {
    ::mf::InjectMe::advanced::reset();
  }

  void TearDown() override {
    ::mf::InjectMe::advanced::reset();
  }
};

class ConfigureWithProvider : public ConfigureTestsBase {};

TEST_F(ConfigureWithProvider, itCanConfigureWithoutParameters) {
  EXPECT_NO_THROW(configure<int>().done());
}

TEST_F(ConfigureWithProvider, itCanConfigureWithValidProvider) {
  EXPECT_NO_THROW(configure<int>().setProvider(validProvider));
}

TEST_F(ConfigureWithProvider, itThrowsOnNullProvider) {
  EXPECT_THROW(configure<int>().setProvider(nullptr).done(), exceptions::MissingProvider);
}

TEST_F(ConfigureWithProvider, itCanConfigureWithFailingProvider) {
  EXPECT_NO_THROW(configure<int>().setProvider(failingProvider));
}

TEST_F(ConfigureWithProvider, itThrowsOnDuplicateForType) {
  EXPECT_NO_THROW(configure<int>().done());
  EXPECT_THROW(configure<int>().setProvider(validProvider).done(), exceptions::DuplicateProvider);
}

TEST_F(ConfigureWithProvider, itCanConfigureWithDeleter) {
  EXPECT_NO_THROW(configure<int>().setDeleter(validDeleter).done());
}

TEST_F(ConfigureWithProvider, itCanConfigureWithNullDeleter) {
  EXPECT_NO_THROW(configure<int>().setDeleter(nullptr).done());
}

class ConfigureWithValue : public ConfigureTestsBase {
 protected:
  int* validInt = new int(2);
};

TEST_F(ConfigureWithValue, itCanConfigureWithValue) {
  EXPECT_NO_THROW(configureWithValue<int>(validInt).done());
}

TEST_F(ConfigureWithValue, itThrowsOnNullValue) {
  EXPECT_THROW(configureWithValue<int>(nullptr).done(), exceptions::InvalidPointer);
}

TEST_F(ConfigureWithValue, itCanConfigureWithDeleter) {
  EXPECT_NO_THROW(configureWithValue<int>(validInt).setDeleter(validDeleter).done());
}

TEST_F(ConfigureWithValue, itCanConfigureWithNullDeleter) {
  EXPECT_NO_THROW(configureWithValue<int>(validInt).setDeleter(nullptr).done());
}

TEST_F(ConfigureWithValue, itThrowsOnDuplicateForType) {
  EXPECT_NO_THROW(configureWithValue<int>(validInt).done());
  int* anotherInt = new int(42);
  auto configureHelper = configureWithValue<int>(anotherInt);
  EXPECT_THROW(configureHelper.setDeleter(validDeleter).done(), exceptions::DuplicateProvider);
}
