set(PROJECT_NAME "Test")

#include in the build all the content inside the directory
include_directories("../include")
include_directories("../../main/include")

#you might want to add the sources via the following command: set(SOURCES src/mainapp.cpp src/Student.cpp)
#but with GLOB is all much easier; include in the build all the content filtered by the pattern

file(GLOB SOURCES "*.c")

#create the library called "EZGraph" from all the files labelled with "SOURCES"
add_executable(${PROJECT_NAME} ${SOURCES})
link_directories(${CMAKE_BINARY_DIR})
target_link_libraries(${PROJECT_NAME} "CrashC")

set_target_properties(${PROJECT_NAME}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)