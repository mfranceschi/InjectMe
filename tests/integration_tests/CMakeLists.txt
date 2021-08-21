
set(Integration_Tests_Folders
  configure_and_exit_0
  configure_complex_type_0
  configure_single_type_0
  dummy_function_0
  recursion_is_caught
  triple_nested_injections
)

find_program(
  VALGRIND_PROGRAM 
  NAMES valgrind
)
set(VALGRIND_OPTIONS --leak-check=full --show-leak-kinds=all --error-exitcode=44 )

foreach(Integration_Test_Folder ${Integration_Tests_Folders})
  add_subdirectory(${Integration_Test_Folder})
  add_test(
    NAME "Integration_Test_${Integration_Test_Folder}"
    COMMAND $<TARGET_FILE:${Integration_Test_Folder}>
  )

  if(VALGRIND_PROGRAM)
    add_test(
      NAME "Integration_Test_${Integration_Test_Folder}_Memcheck_Valgrind"
      COMMAND valgrind ${VALGRIND_OPTIONS} $<TARGET_FILE:${Integration_Test_Folder}>
    )
    set_tests_properties(
        "Integration_Test_${Integration_Test_Folder}_Memcheck_Valgrind" 
        PROPERTIES 
          DEPENDS "Integration_Test_${Integration_Test_Folder}"
    )
  endif()
endforeach()

get_property(Integration_Tests DIRECTORY . PROPERTY TESTS)
foreach(In_Test ${Integration_Tests})
  set_tests_properties(
    ${In_Test} 
    PROPERTIES 
      LABELS "IntegrationTests"
      TIMEOUT 2
  )
endforeach()
