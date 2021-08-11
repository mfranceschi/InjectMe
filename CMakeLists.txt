cmake_minimum_required(VERSION 3.16)
project(Mfranceschi_InjectMe LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(src)

enable_testing()
add_subdirectory(tests)
