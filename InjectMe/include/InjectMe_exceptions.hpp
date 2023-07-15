#pragma once

#include <stdexcept>
#include <string>

namespace mf
{
  namespace InjectMe
  {
    namespace exceptions
    {
      class Exception : public std::exception {
       public:
        Exception(
            const std::string& failingComponent,
            const std::string& errorName,
            const std::string& errorDetails);

        const char* what() const noexcept override;

       private:
        std::string failingComponent;
        std::string errorName;
        std::string errorDetails;
        std::string resultingString;
      };

      /**
       * Thrown if the pointer is invalid; often null.
       */
      class InvalidPointer : public Exception {
       public:
        InvalidPointer(const std::string& failingComponent, const std::string& errorDetails = "");
      };

      /**
       * No available (default or declared) provider for the given type.
       */
      class MissingProvider : public Exception {
       public:
        MissingProvider(const std::string& failingComponent, const char* typeName);
      };

      /**
       * There is more than one provider for that type.
       */
      class DuplicateProvider : public Exception {
       public:
        DuplicateProvider(const std::string& failingComponent, const char* typeName);
      };

      /**
       * A recursion has been detected and fails fast.
       * Example: when building A, we need B, so we build B but to build B we need A.
       */
      class ProviderRecursion : public Exception {
       public:
        ProviderRecursion(const std::string& failingComponent, const char* typeName);
      };

      class DoneHasNotBeenCalled : public Exception {
       public:
        DoneHasNotBeenCalled(const char* typeName);
      };

      class Internal : public Exception {
       public:
        Internal(const std::string& errorDetails);
      };
    }  // namespace exceptions

    using Exc = exceptions::Exception;
  }  // namespace InjectMe
}  // namespace mf
