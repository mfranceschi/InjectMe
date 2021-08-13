
add_test(
    NAME intest_dummy_function_0
    COMMAND 
        sh ${CMAKE_CURRENT_LIST_DIR}/testing.sh 
        0 
        $<TARGET_FILE:MF_InjectMe> 
        ${CMAKE_SOURCE_DIR}/InjectMe/include 
        ${CMAKE_BUILD_TYPE} 
        ${CMAKE_CURRENT_LIST_DIR}/dummy_function_0
)

get_property(Integration_Tests DIRECTORY . PROPERTY TESTS)
foreach(In_Test ${Integration_Tests})
  set_tests_properties(${In_Test} PROPERTIES LABELS "IntegrationTests")
endforeach()
