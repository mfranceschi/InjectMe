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

      class InvalidPointer : public Exception {
       public:
        InvalidPointer(const std::string& failingComponent, const std::string& errorDetails = "");
      };

      class MissingProvider : public Exception {
       public:
        MissingProvider(const std::string& failingComponent, const char* typeName);
      };

      class DuplicateProvider : public Exception {
       public:
        DuplicateProvider(const std::string& failingComponent, const char* typeName);
      };

      class ProviderRecursion : public Exception {
       public:
        ProviderRecursion(const std::string& failingComponent, const char* typeName);
      };

      class Internal : public Exception {
       public:
        Internal(const std::string& errorDetails);
      };
    }  // namespace exceptions

    using Exc = exceptions::Exception;
  }  // namespace InjectMe
}  // namespace mf
