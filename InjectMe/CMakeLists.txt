
add_library(MF_InjectMe STATIC)

set(PUBLIC_HEADERS
    include/InjectMe.hpp
)
set(PRIVATE_HEADERS
    src/ConfigImpl.hpp
)
set(PRIVATE_IMPLEMENTATIONS
    src/Config.cpp
    src/ConfigImpl.cpp
    src/dummy_function.cpp
)

target_sources(
    MF_InjectMe
    PUBLIC ${PUBLIC_HEADERS}
    PRIVATE ${PRIVATE_HEADERS} ${PRIVATE_IMPLEMENTATIONS}
)
target_include_directories(
    MF_InjectMe
    PUBLIC ./include/
    PRIVATE ./src/
)
