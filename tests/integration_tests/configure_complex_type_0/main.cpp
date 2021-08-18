#include <cmath>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>

#include "../IntegrationTestHelpers.hpp"
#include "InjectMe.hpp"

static constexpr int THE_VALUE = 42;

class MyType {
 public:
  MyType() : pointer(new int(THE_VALUE)) {
  }
  MyType(const MyType&) = delete;
  MyType& operator=(const MyType&) = delete;
  MyType(MyType&&) = delete;
  MyType& operator=(MyType&&) = delete;
  ~MyType() {
    delete pointer;
  }

 private:
  const int* const pointer;
};

void configureInjector() {
  auto injectMeConfig = mf::InjectMe::Config::getInstance();

  injectMeConfig->add<MyType>([]() {
    callCounter++;
    return new MyType();
  });

  configure(injectMeConfig);
}

void runChecks() {
  auto checkValue = []() {
    auto injected = mf::InjectMe::inject<MyType>();
    static_assert(std::is_same<decltype(*injected), MyType&>::value, "Surprising type");
  };

  checkCallCount(0);
  checkValue();
  checkCallCount(1);
  checkValue();
  checkCallCount(1);
  checkValue();
  checkCallCount(1);
}

int main() {
  configureInjector();
  runChecks();
  return 0;
}
