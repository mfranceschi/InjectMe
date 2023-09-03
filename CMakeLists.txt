cmake_minimum_required(VERSION 3.16)
project(Mfranceschi_InjectMe LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)

if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

add_subdirectory(InjectMe)

include(CTest)
if (BUILD_TESTING)
  add_subdirectory(tests)
endif()
