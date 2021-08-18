#include <cmath>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>

inline void myAssert(bool expressionToTest, const std::string& errorMessage) {
  if (!expressionToTest) {
    std::cerr << "Error: " << errorMessage << std::endl;
    std::exit(1);
  }
}

inline bool doublesAreAlmostEqual(double x, double y) {
  // Source = https://en.cppreference.com/w/cpp/types/numeric_limits/epsilon
  static constexpr auto ULP = 2;
  return std::fabs(x - y) <= std::numeric_limits<double>::epsilon() * std::fabs(x + y) * ULP
         // unless the result is subnormal
         || std::fabs(x - y) < std::numeric_limits<double>::min();
}

static unsigned int callCounter = 0;

inline void checkCallCount(unsigned int expectedCallCount) {
  bool testResult = callCounter == expectedCallCount;

  std::ostringstream errorMessageStream;
  errorMessageStream << "Unexpected call count (expected " << expectedCallCount
                     << "): " << callCounter;
  auto errorMessage = errorMessageStream.str();

  myAssert(testResult, errorMessage);
}
