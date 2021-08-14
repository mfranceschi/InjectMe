#include <iostream>
#include <string>

#include "InjectMe.hpp"

void configureInjector() {
  using namespace mf::InjectMe;

  auto injectMeConfig = Config::getInstance();

  Config::ProviderFct<std::string> stringProvider = []() {
    std::cout << "Called the string provider function." << std::endl;
    return new std::string("Hello, world!");
  };
  injectMeConfig->add<std::string>(stringProvider);

  Config::ProviderFct<double> doubleProvider = []() {
    std::cout << "Called the double provider function." << std::endl;
    return new double(10. / 3.);
  };
  injectMeConfig->add<double>(doubleProvider);

  configure(injectMeConfig);
}

int main() {
  configureInjector();
  return 0;
}
