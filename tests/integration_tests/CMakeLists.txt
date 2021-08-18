
set(Integration_Tests_Folders
  dummy_function_0
  configure_and_exit_0
)

find_program(
  VALGRIND_PROGRAM 
  NAMES valgrind
)

foreach(Integration_Test_Folder ${Integration_Tests_Folders})
  add_subdirectory(${Integration_Test_Folder})
  add_test(
    NAME "Integration_Test_${Integration_Test_Folder}"
    COMMAND $<TARGET_FILE:${Integration_Test_Folder}>
  )

  if(VALGRIND_PROGRAM)
    add_test(
      NAME "Integration_Test_${Integration_Test_Folder}_Memcheck_Valgrind"
      COMMAND valgrind --leak-check=full --error-exitcode=44 $<TARGET_FILE:${Integration_Test_Folder}>
    )
  endif()
  
endforeach()

get_property(Integration_Tests DIRECTORY . PROPERTY TESTS)
foreach(In_Test ${Integration_Tests})
  set_tests_properties(${In_Test} PROPERTIES LABELS "IntegrationTests")
endforeach()
