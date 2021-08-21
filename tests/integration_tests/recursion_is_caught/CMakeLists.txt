
add_executable(recursion_is_caught)
target_link_libraries(recursion_is_caught PUBLIC MF_InjectMe)

target_sources(recursion_is_caught PRIVATE main.cpp)
