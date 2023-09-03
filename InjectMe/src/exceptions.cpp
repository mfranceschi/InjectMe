#include <sstream>

#include "InjectMe.hpp"
#include "InjectMe_exceptions.hpp"

namespace mf
{
  namespace InjectMe
  {
    namespace exceptions
    {
      namespace
      {
        std::string makeErrorDetailsFromTypeName(const char* typeName) {
          std::stringstream stream;
          stream << "type is '" << typeName << ".";
          return stream.str();
        }
      }  // namespace

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
                makeErrorDetailsFromTypeName(typeName)) {
      }

      DuplicateProvider::DuplicateProvider(
          const std::string& failingComponent, const char* typeName)
          : Exception(
                failingComponent,
                "too many (2+) providers for this type",
                makeErrorDetailsFromTypeName(typeName)) {
      }

      ProviderRecursion::ProviderRecursion(
          const std::string& failingComponent, const char* typeName)
          : Exception(
                failingComponent,
                "recursion while calling providers",
                makeErrorDetailsFromTypeName(typeName)) {
      }

      Internal::Internal(const std::string& errorDetails)
          : Exception("(internal)", "Internal error", errorDetails) {
      }

      DoneHasNotBeenCalled::DoneHasNotBeenCalled(const char* typeName)
          : Exception(
                "configure", "done has not been called", makeErrorDetailsFromTypeName(typeName)) {
      }
    }  // namespace exceptions
  }  // namespace InjectMe
}  // namespace mf
