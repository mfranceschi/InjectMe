EXPECTED_EXIT_CODE=$1
INJECTME_LIB_FILE=$2 
INJECTME_INCLUDE_DIRS=$3 
CMAKE_BUILD_TYPE=$4
SOURCE_DIR=$5

BUILD_FOLDER=./build0

# Ensure there is no build folder
rm -rf ${BUILD_FOLDER}

# CMake configure and build
INJECTME_LIB_FILE=${INJECTME_LIB_FILE} INJECTME_INCLUDE_DIRS=${INJECTME_INCLUDE_DIRS} cmake -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -S ${SOURCE_DIR} -B ${BUILD_FOLDER} && cmake --build ${BUILD_FOLDER}
test $? -eq 0 || (echo "CMake fail for source dir ${SOURCE_DIR}." && exit 1)
test -f ${BUILD_FOLDER}/Main0 || (echo "Missing executable file after CMake build for source dir ${SOURCE_DIR}." && exit 1)

# Run program
${BUILD_FOLDER}/Main0
EXECUTABLE_EXIT_CODE=$?
test ${EXECUTABLE_EXIT_CODE} -eq ${EXPECTED_EXIT_CODE} || (echo "Executable exited with an unexpected error code ${EXECUTABLE_EXIT_CODE} (expected ${EXPECTED_EXIT_CODE}) for source dir ${SOURCE_DIR}" && exit 1)

# Run valgrind if possible
if [ ${EXPECTED_EXIT_CODE} -eq 0 ]; then
    valgrind --leak-check=full --error-exitcode=44 ${BUILD_FOLDER}/Main0
    test $? -eq 0 || (echo "Valgrind found errors while running executable for source dir ${SOURCE_DIR}" && exit 1)
fi

# Remove build folder
rm -rf ${BUILD_FOLDER}