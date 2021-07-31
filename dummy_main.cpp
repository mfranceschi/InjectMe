#include <iostream>
#include <memory>

int main() {
    std::unique_ptr<int> pointer = std::make_unique<int>(42);
    std::cout << "Hello, world! " << *pointer << std::endl;
    return EXIT_SUCCESS;
}
