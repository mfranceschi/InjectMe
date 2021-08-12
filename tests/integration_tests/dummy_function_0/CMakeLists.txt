cmake_minimum_required(VERSION 3.16)
project(Integration_Test)
add_executable(Main0)
target_link_libraries(Main0 PUBLIC $ENV{INJECTME_LIB_FILE})
target_include_directories(Main0 PUBLIC $ENV{INJECTME_INCLUDE_DIRS})

target_sources(Main0 PRIVATE main.cpp)
