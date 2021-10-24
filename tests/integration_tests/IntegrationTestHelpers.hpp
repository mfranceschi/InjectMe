#include <cmath>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

/**
 * If the given @c expressionToTest is false, then print @c errorMessage on stderr and exit.
 * Otherwise no-op.
 */
inline void myAssert(bool expressionToTest, const std::string& errorMessage) {
  if (!expressionToTest) {
    std::cerr << "Error: " << errorMessage << std::endl;
    std::exit(1);
  }
}

/**
 * Checks for close proximity of two double values.
 */
inline bool doublesAreAlmostEqual(double x, double y) {
  // Source = https://en.cppreference.com/w/cpp/types/numeric_limits/epsilon
  static constexpr auto ULP = 2;
  return std::fabs(x - y) <= std::numeric_limits<double>::epsilon() * std::fabs(x + y) * ULP
         // unless the result is subnormal
         || std::fabs(x - y) < std::numeric_limits<double>::min();
}

class CallCounter {
 public:
  CallCounter() = default;
  void addCall(const std::string& text = "(no text)") {
    calls.push_back(text);
  }
  std::size_t getCounter() const {
    return calls.size();
  }
  bool checkCounterWith(const std::size_t& value) const {
    return getCounter() == value;
  }
  std::string getCallTextAtIndex(const std::size_t& index) const {
    return calls.at(index);
  }

 private:
  std::vector<std::string> calls;
};

inline CallCounter& getCallCounter() {
  static CallCounter cc;
  return cc;
}

/**
 * Helper for testing if @c callCounter==expectedCallCount.
 */
inline void checkCallCount(unsigned int expectedCallCount) {
  bool testResult = getCallCounter().checkCounterWith(expectedCallCount);

  std::ostringstream errorMessageStream;
  errorMessageStream << "Unexpected call count: expected " << expectedCallCount << ", got "
                     << getCallCounter().getCounter();
  auto errorMessage = errorMessageStream.str();

  myAssert(testResult, errorMessage);
}

// Use this to delete copy and move constructors and assignment operators.
#define NO_COPY(className)                         \
  className(const className&) = delete;            \
  className& operator=(const className&) = delete; \
  className(className&&) = delete;                 \
  className& operator=(className&&) = delete
