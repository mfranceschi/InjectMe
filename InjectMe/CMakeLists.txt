
add_library(MF_InjectMe STATIC)

set(PUBLIC_HEADERS
    include/InjectMe_exceptions.hpp
    include/InjectMe_types.hpp
    include/InjectMe.hpp
)
set(PRIVATE_HEADERS
    src/Database.hpp
    src/DatabaseInstanceInsertion.hpp
    src/TypeData.hpp
)
set(PRIVATE_IMPLEMENTATIONS
    src/configure.cpp
    src/Database.cpp
    src/DatabaseInstanceInsertion.cpp
    src/dummy_function.cpp
    src/exceptions.cpp
    src/inject.cpp
    src/reset.cpp
    src/TypeData.cpp
)

target_sources(
    MF_InjectMe
    PUBLIC ${PUBLIC_HEADERS}
    PRIVATE ${PRIVATE_HEADERS} ${PRIVATE_IMPLEMENTATIONS}
)
target_include_directories(
    MF_InjectMe
    PUBLIC include
    PRIVATE src
)
