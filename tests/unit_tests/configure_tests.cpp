#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <stdexcept>

#include "InjectMe.hpp"

using namespace mf::InjectMe;

#if 0  // TODO fix tests according to new config API
TEST(Configure, itThrowsIfPointerIsNull) {
  Config::ConfigPtr configPtr = nullptr;
  ASSERT_THROW(configure(configPtr), exceptions::InvalidPointer);
}

TEST(Configure, itThrowsIfNoProviderHasBeenSet) {
  GTEST_SKIP() << "Deprecated";
  Config::ConfigPtr configPtr = Config::getInstance();
  ASSERT_THROW(configure(configPtr), std::logic_error);
}

class MockConfig : public Config {
 public:
  MOCK_METHOD(
      void,
      setProviderForTypeOrThrow,
      (const ProviderFct<void>&, const std::type_info&, const Deleter&),
      (override));
};

TEST(Configure, itThrowsIfInstanceIsNotConfigImpl) {
  Config::ConfigPtr configPtr = std::make_unique<MockConfig>();
  ASSERT_THROW(configure(configPtr), exceptions::Internal);
}
#endif
