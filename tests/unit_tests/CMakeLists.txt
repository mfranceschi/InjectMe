include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(
    InjectMe_GoogleTests
    Config_tests.cpp
    configure_tests.cpp
    dummy_function_tests.cpp
    tests_main.cpp
)
target_link_libraries(InjectMe_GoogleTests gtest_main gmock MF_InjectMe)

include(GoogleTest)
gtest_discover_tests(
    InjectMe_GoogleTests
    TEST_PREFIX UnitTests_
    PROPERTIES
        LABELS "UnitTests"
)
