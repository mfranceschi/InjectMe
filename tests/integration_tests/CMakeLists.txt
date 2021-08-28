
set(Integration_Tests_Folders
  0_dummy_function
  1_configure_and_exit
  2_configure_single_type
  3_configure_complex_type
  4_triple_nested_injections
  5_recursion_is_caught
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
      TIMEOUT 5
  )
endforeach()
