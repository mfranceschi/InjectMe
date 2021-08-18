
add_test(
    NAME intest_dummy_function_0
    COMMAND
        sh ${CMAKE_CURRENT_LIST_DIR}/testing.sh
        0 # Expected exit code
        $<TARGET_FILE:MF_InjectMe> # Library binary file
        ${CMAKE_SOURCE_DIR}/InjectMe/include # Library include file
        ${CMAKE_BUILD_TYPE}
        ${CMAKE_CURRENT_LIST_DIR}/dummy_function_0 # Directory to run tests in
)

add_test(
    NAME intest_configure_and_exit_0
    COMMAND
        sh ${CMAKE_CURRENT_LIST_DIR}/testing.sh
        0 # Expected exit code
        $<TARGET_FILE:MF_InjectMe> # Library binary file
        ${CMAKE_SOURCE_DIR}/InjectMe/include # Library include file
        ${CMAKE_BUILD_TYPE}
        ${CMAKE_CURRENT_LIST_DIR}/configure_and_exit_0 # Directory to run tests in
)

get_property(Integration_Tests DIRECTORY . PROPERTY TESTS)
foreach(In_Test ${Integration_Tests})
  set_tests_properties(${In_Test} PROPERTIES LABELS "IntegrationTests")
endforeach()
