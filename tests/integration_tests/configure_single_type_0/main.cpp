#include <cmath>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>

#include "InjectMe.hpp"

static constexpr double THE_VALUE = 3.1415;
static unsigned int callCounter = 0;

void myAssert(bool expressionToTest, const std::string& errorMessage) {
  if (!expressionToTest) {
    std::cerr << "Error: " << errorMessage << std::endl;
    std::exit(1);
  }
}

bool doublesAreAlmostEqual(double x, double y) {
  // Source = https://en.cppreference.com/w/cpp/types/numeric_limits/epsilon
  static constexpr auto ULP = 2;
  return std::fabs(x - y) <= std::numeric_limits<double>::epsilon() * std::fabs(x + y) * ULP
         // unless the result is subnormal
         || std::fabs(x - y) < std::numeric_limits<double>::min();
}

void configureInjector() {
  auto injectMeConfig = mf::InjectMe::Config::getInstance();

  injectMeConfig->add<double>([]() {
    callCounter++;
    return new double(THE_VALUE);
  });

  configure(injectMeConfig);
}

void runChecks() {
  auto checkCallCount = [](unsigned int expectedCallCount) {
    bool testResult = callCounter == expectedCallCount;

    std::ostringstream errorMessageStream;
    errorMessageStream << "Unexpected call count (expected " << expectedCallCount
                       << "): " << callCounter;
    auto errorMessage = errorMessageStream.str();

    myAssert(testResult, errorMessage);
  };

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
