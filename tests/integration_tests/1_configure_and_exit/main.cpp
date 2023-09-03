#include <iostream>
#include <string>

#include "InjectMe.hpp"

void configureInjector() {
  using namespace mf::InjectMe;

  configure<std::string>().setProvider([]() {
    std::cout << "Called the string provider function." << std::endl;
    return new std::string("Hello, world!");
      })
      .done();
  configure<double>().setProvider([]() {
    std::cout << "Called the double provider function." << std::endl;
    return new double(10. / 3.);
  }).done();
}

int main() {
  configureInjector();
  return 0;
}
