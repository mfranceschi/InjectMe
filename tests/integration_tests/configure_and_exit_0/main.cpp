#include <iostream>
#include <string>

#include "InjectMe.hpp"

void configureInjector() {
  using namespace mf::InjectMe;

  auto injectMeConfig = Config::getInstance();

  injectMeConfig
      ->add<std::string>([]() {
        std::cout << "Called the string provider function." << std::endl;
        return new std::string("Hello, world!");
      })
      ->add<double>([]() {
        std::cout << "Called the double provider function." << std::endl;
        return new double(10. / 3.);
      });

  configure(injectMeConfig);
}

int main() {
  configureInjector();
  return 0;
}
