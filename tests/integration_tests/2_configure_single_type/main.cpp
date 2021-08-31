#include <cmath>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>

#include "../IntegrationTestHelpers.hpp"
#include "InjectMe.hpp"

static constexpr double THE_VALUE = 3.1415;

void configureInjector() {
  using namespace mf::InjectMe;

  configure<double>()
      .setProvider([]() {
        callCounter++;
        return new double(THE_VALUE);
      })
      .done();
}

void runChecks() {
  auto checkValue = []() {
    auto injected = mf::InjectMe::inject<double>();
    bool testResult = doublesAreAlmostEqual(*injected, THE_VALUE);

    std::ostringstream errorMessageStream;
    errorMessageStream << "Unexpected value (expected " << THE_VALUE << "): " << *injected;
    auto errorMessage = errorMessageStream.str();

    myAssert(testResult, errorMessage);
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
