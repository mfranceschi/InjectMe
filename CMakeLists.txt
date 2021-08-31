cmake_minimum_required(VERSION 3.16)
project(Mfranceschi_InjectMe LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  add_compile_options(-Wall -Wextra -Wabi=11 -Wpedantic)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	add_compile_options(/W4)
endif()

add_subdirectory(InjectMe)

enable_testing()
add_subdirectory(tests)
