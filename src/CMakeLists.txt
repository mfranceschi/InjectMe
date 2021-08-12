
add_library(MF_InjectMe STATIC)

set(PUBLIC_HEADERS
    include/InjectMe.hpp
)
set(PRIVATE_HEADERS
)
set(PRIVATE_IMPLEMENTATIONS
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
