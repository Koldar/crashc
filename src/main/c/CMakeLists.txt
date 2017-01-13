
#include in the build all the content inside the directory
include_directories("../include")
#you might want to add the sources via the following command: set(SOURCES src/mainapp.cpp src/Student.cpp)
#but with GLOB is all much easier; include in the build all the content filtered by the pattern
file(GLOB SOURCES "*.c")
file(GLOB HEADERS "../include/*.h")

#create the library called "EZGraph" from all the files labelled with "SOURCES"
add_library(${PROJECT_NAME} SHARED ${SOURCES})

set_target_properties(${PROJECT_NAME}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)

include(GNUInstallDirs)

#when user do "make install" this line will be used. Determine where the library will be placed
install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(FILES ${HEADERS} DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}")
#run ldconfig to update the cache with the new installed library
install(CODE "execute_process(COMMAND ldconfig)")