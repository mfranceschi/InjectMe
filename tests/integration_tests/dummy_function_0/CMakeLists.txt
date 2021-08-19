
add_executable(dummy_function_0)
target_link_libraries(dummy_function_0 PUBLIC MF_InjectMe)

target_sources(dummy_function_0 PRIVATE main.cpp)
