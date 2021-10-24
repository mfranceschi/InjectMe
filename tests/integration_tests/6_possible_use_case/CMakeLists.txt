
get_filename_component(Integration_Test_Name ${CMAKE_CURRENT_SOURCE_DIR} NAME)
add_executable(${Integration_Test_Name})
target_link_libraries(${Integration_Test_Name} PUBLIC MF_InjectMe)

target_sources(${Integration_Test_Name} PRIVATE main.cpp)
