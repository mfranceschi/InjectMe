#include <gtest/gtest.h>

#include <stdexcept>

#include "InjectMe.hpp"

#if 0  // TODO fix tests according to new API
using mf::InjectMe::Config;
using mf::InjectMe::ProviderFct;

TEST(Config, itCanGetInstance) {
  std::unique_ptr<Config> config1;
  std::unique_ptr<Config> config2;
  ASSERT_NO_THROW(config1 = Config::getInstance());
  ASSERT_NO_THROW(config2 = Config::getInstance());

  ASSERT_NE(config1.get(), config2.get());
}

class ConfigAddIntType : public ::testing::Test {
 protected:
  using TypeToAdd = int;
  static constexpr int VALUE = 42;
  ProviderFct<int> intTypeProvider = []() {
    return new int(VALUE);
  };
  std::unique_ptr<Config> config;

  void SetUp() override {
    auto* providedValue = intTypeProvider();
    EXPECT_TRUE(*providedValue == VALUE);
    delete providedValue;
    config = Config::getInstance();
  }
};

TEST_F(ConfigAddIntType, itCanAdd) {
  ASSERT_NO_THROW(config->add<int>(intTypeProvider));
}

TEST_F(ConfigAddIntType, itThrowsOnDuplicate) {
  config->add<int>(intTypeProvider);
  ASSERT_THROW(config->add<int>(intTypeProvider), mf::InjectMe::exceptions::DuplicateProvider);
}

TEST_F(ConfigAddIntType, itReturnsThisOnAdd) {
  ASSERT_EQ(config->add<int>(intTypeProvider), config.get());
}

#endif
