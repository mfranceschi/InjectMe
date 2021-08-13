#include <gtest/gtest.h>

#include <stdexcept>

#include "InjectMe.hpp"

using mf::InjectMe::Config;

TEST(Config, itCanGetInstance) {
  std::unique_ptr<Config> config1, config2;
  ASSERT_NO_THROW(config1 = Config::getInstance());
  ASSERT_NO_THROW(config2 = Config::getInstance());

  ASSERT_NE(config1.get(), config2.get());
}

class ConfigAddIntType : public ::testing::Test {
 protected:
  using TypeToAdd = int;
  static constexpr int VALUE = 42;
  Config::ProviderPtr<int> intTypeProvider = []() { return new int(VALUE); };
  std::unique_ptr<Config> config;

  void SetUp() override {
    auto providedValue = intTypeProvider();
    EXPECT_TRUE(*providedValue == VALUE);
    delete providedValue;
    config = Config::getInstance();
  }
};

TEST_F(ConfigAddIntType, itCanAdd) {
  ASSERT_NO_THROW(config->add<int>(intTypeProvider));
}

TEST_F(ConfigAddIntType, itThrowsOnDuplicate) {
  GTEST_SKIP() << "Waiting for the exception type to check.";
  config->add<int>(intTypeProvider);
  ASSERT_THROW(config->add<int>(intTypeProvider), std::logic_error);
}
