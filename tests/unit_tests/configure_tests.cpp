#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <stdexcept>

#include "InjectMe.hpp"

using mf::InjectMe::Config;
using mf::InjectMe::configure;

TEST(Configure, itThrowsIfPointerIsNull) {
  Config::ConfigPtr configPtr = nullptr;
  ASSERT_THROW(configure(configPtr), std::invalid_argument);
}

TEST(Configure, itThrowsIfNoProviderHasBeenSet) {
  Config::ConfigPtr configPtr = Config::getInstance();
  ASSERT_THROW(configure(configPtr), std::logic_error);
}

class MockConfig : public Config {
 public:
  MOCK_METHOD(void, setProviderForTypeOrThrow, (const ProviderFct<void>&, const std::type_info&),
              (override));
};

TEST(Configure, itThrowsIfInstanceIsNotConfigImpl) {
  Config::ConfigPtr configPtr = std::make_unique<MockConfig>();
  ASSERT_THROW(configure(configPtr), std::invalid_argument);
}
