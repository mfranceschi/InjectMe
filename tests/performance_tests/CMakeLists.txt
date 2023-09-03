find_package(Python3)
if (Python3_FOUND)
    set(NBR_CLASSES_TO_GO_THROUGH 10 100 300 500)
    foreach (NBR_CLASSES IN LISTS NBR_CLASSES_TO_GO_THROUGH)
        set(T_${NBR_CLASSES}_Classes_CppFile ${CMAKE_CURRENT_BINARY_DIR}/T_${NBR_CLASSES}_Classes_CppFile.cpp)
        add_custom_command(
                OUTPUT ${T_${NBR_CLASSES}_Classes_CppFile}
                COMMAND Python3::Interpreter ${CMAKE_CURRENT_SOURCE_DIR}/generate_cpp_file.py --nbrClasses=${NBR_CLASSES} --targetFile="${T_${NBR_CLASSES}_Classes_CppFile}"
                COMMENT "Generating file!"
        )
        add_executable(T_${NBR_CLASSES}_Classes ${T_${NBR_CLASSES}_Classes_CppFile})
        target_link_libraries(T_${NBR_CLASSES}_Classes PRIVATE MF_InjectMe)
        target_compile_options(T_${NBR_CLASSES}_Classes PRIVATE /bigobj)
    endforeach ()
endif ()
