#include <sstream>

#include "InjectMe.hpp"

namespace mf
{
  namespace InjectMe
  {
    namespace exceptions
    {
      Exception::Exception(
          const std::string& failingComponent,
          const std::string& errorName,
          const std::string& errorDetails)
          : failingComponent(failingComponent), errorName(errorName), errorDetails(errorDetails) {
        std::ostringstream oss;
        oss << "InjectMe::" << failingComponent << " - " << errorName;
        if (!errorDetails.empty()) {
          oss << " - " << errorDetails;
        }
        resultingString = oss.str();
      }

      const char* Exception::what() const noexcept {
        return resultingString.c_str();
      }

      InvalidPointer::InvalidPointer(
          const std::string& failingComponent, const std::string& errorDetails)
          : Exception(failingComponent, "invalid pointer", errorDetails) {
      }

      MissingProvider::MissingProvider(const std::string& failingComponent, const char* typeName)
          : Exception(
                failingComponent,
                "missing provider for this type",
                std::string("type is '") + std::string(typeName) + std::string("'.")) {
      }

      DuplicateProvider::DuplicateProvider(
          const std::string& failingComponent, const char* typeName)
          : Exception(
                failingComponent,
                "too many (2+) providers for this type",
                std::string("type is '") + std::string(typeName) + std::string("'.")) {
      }
    }  // namespace exceptions
  }    // namespace InjectMe
}  // namespace mf
