# **********************************************************************************************
# ************************* MAIN PROPERTIES (YOU NEED TO EDIT THEM!!) **************************
# **********************************************************************************************

#the name of the project
set(THEPROJECT_NAME "CUtils")
#the version of the project
set(THEPROJECT_VERSION 1.0)
#what will be prodiced: either EXE (executable); SO (shared library) AO (static library)
#Can be overriden by using "cmake -DU_LIBRARY_TYPE:STRING=<newvalue>" command
set(THEPROJECT_OUTPUT "SO")
#a spaced separated list of shared libraries that will be used when linking the main project. Each library needs to be installed
#on the system. Each library should be declared as a quoted string
set(THEPROJECT_REQUIRED_SHARED_LIBRARIES "m" "rt")
#a spaced separated list of additional shared libraries that will be used when linking the test application. Each library needs to be installed
#ignore it if you put "THEPROJECT_TEST_ENABLE_TEST_COMPILATION" to "false" 
set(THEPROJECT_TEST_ADDITIONAL_SHARED_LIBRARIES "")
#true if you want to compile the all the tests inside src/test/c src/test/include.
#values: "true", "false"
set(THEPROJECT_TEST_ENABLE_TEST_COMPILATION "true")
#If you're building a library, use this variable to enable or disable the -fPIC flag. Ignored if not building library.
#turning on will allow multiple process to share the same library object code but it will reduce performances.
#By turning off every process using the library will have its own copy of the library code, but it will increase performances.
#Can be overriden by using "cmake -DU_FPIC:STRING=<newvalue>" command  
set(THEPROJECT_POSITION_INDEPENDENT_CODE "true")
#put true if you have changed something inside this cmake standard building process; false otherwise
set(STANDARD_CMAKE_FILE_ALTERED "false")
#If you have altered the standard CMAKE file standard process, consider explaining in this variable what have you changed to help future maintainers!
#The variable is ignored if "STANDARD_CMAKE_FILE_ALTERED" is false
set(CMAKE_FILE_ALTERED_COMMAND "The author said nothing about the changes on the cmake file it made. That little punk! >:(")
#Represents the version of the building process version. You can use this value to understand what this cmake building process can and can't do
#For example in building processes before the "1.0" "sudo make install" of exectuables wasn't supported.
# - 1.0: first version
# - 1.1: sudo make install for static libraries as well
# - 1.2: cmake version log change
# - 1.3: added position independent code, cmake command line arguments
# - 1.4: "make install/uninstall" works without root access; Use U_INSTALL_DIRECTORY to alter installation directory; refactored  messages
set(CMAKE_FILE_BUILDING_PROCESS_VERSION "1.4")
#The place where all the stuff created by "sudo make install" will be positioned"
#Leave it if you don't want to change the behaviour. Can be overriden by U_INSTALL_DIRECTORY. For example: "-DU_INSTALL_DIRECTORY=~/usr"
#(no end slash!).
set(THEPROJECT_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")












# ******************** CHECK CONSTRAINTS ***************************

cmake_minimum_required(VERSION 2.8.7)
project(${THEPROJECT_NAME})

# ******************** IMPORTANT INCLUDES **************************

# ******************** OVERRIDING  CHANGABLE VARIABLES ************

SET(U_FPIC "" CACHE STRING "true to enable PIC. False to enable Relocation")
SET(U_LIBRARY_TYPE "" CACHE STRING "SO for shared library, AO for static library")
SET(U_INSTALL_DIRECTORY "" CACHE STRING "The place where everything 'sudo make install' is positioned")

if (NOT ${U_FPIC} STREQUAL "")
    set(THEPROJECT_POSITION_INDEPENDENT_CODE ${U_FPIC})
    message(STATUS "${BoldYellow}changing FPIC to ${THEPROJECT_POSITION_INDEPENDENT_CODE}${ColorReset}")
endif()

if (NOT ${U_LIBRARY_TYPE} STREQUAL "")
    set(THEPROJECT_OUTPUT ${U_LIBRARY_TYPE})
    message(STATUS "${BoldYellow}changing library type to ${THEPROJECT_OUTPUT}${ColorReset}")
endif()

if (NOT ${U_INSTALL_DIRECTORY} STREQUAL "")
    set(THEPROJECT_INSTALL_PREFIX ${U_INSTALL_DIRECTORY})
    message(STATUS "${BoldYellow}changing install directory to ${THEPROJECT_INSTALL_PREFIX}${ColorReset}")
endif()

# ************************ SET DEFINITIVE VARIABLES ***************************

#the place where everything will be install into
SET(CMAKE_INSTALL_PREFIX ${THEPROJECT_INSTALL_PREFIX})
#make the make file always verbose (https://stackoverflow.com/questions/4808303/making-cmake-print-commands-before-executing)
set(CMAKE_VERBOSE_MAKEFILE on)

get_filename_component(PARENTDIR ${CMAKE_BINARY_DIR} NAME)

# create string constants (https://stackoverflow.com/a/19578320/1887602)
if(NOT WIN32)
    string(ASCII 27 Esc)
    set(ColorReset "${Esc}[m")
    set(BoldRed     "${Esc}[1;31m")
    set(BoldCyan    "${Esc}[1;36m")
    set(BoldYellow  "${Esc}[1;33m")
endif()

# ******************** BUILDING SUMMARY ***************************

message(STATUS "${BoldYellow}You should call cmake when you are in build/Debug or in build/Release. Perform 'mkdir -p build/Debug; cd build/Debug; cmake ../..'${ColorReset}")
message(STATUS "${BoldYellow}This cmake generates a building process with version ${CMAKE_FILE_BUILDING_PROCESS_VERSION}${ColorReset}")
if (${STANDARD_CMAKE_FILE_ALTERED} STREQUAL "true")
    message(STATUS "${BoldYellow}This cmake building process has been altered from the standard one! This means you need to look at the CMakeLists.txt file as well to understand what has been added!${ColorReset}")
    message(STATUS "${BoldYellow}Here's an explanation of the changes:${ColorReset}\n\n")
    message(STATUS "${BoldYellow}${CMAKE_FILE_ALTERED_COMMAND}\n\n${ColorReset}")
endif()

message(STATUS "${BoldCyan}cmake is working in directory ${PARENTDIR}${ColorReset}")
message(STATUS "${BoldCyan}cmake will build your application in ${CMAKE_BINARY_DIR}${ColorReset}")
message(STATUS "${BoldCyan}cmake will 'sudo make install' your application in ${CMAKE_INSTALL_PREFIX}${ColorReset}") 
if(${THEPROJECT_OUTPUT} STREQUAL "SO")
    message(STATUS "${BoldCyan}We will build a shared library${ColorReset}")
elseif(${THEPROJECT_OUTPUT} STREQUAL "AO")
    message(STATUS "${BoldCyan}We will build a static library${ColorReset}")
elseif(${THEPROJECT_OUTPUT} STREQUAL "EXE")
    message(STATUS "${BoldCyan}We will build an executable${ColorReset}")
endif()


# ******************** BUILDING OPTIONS ***************************

if(PARENTDIR STREQUAL "Release")
    message(STATUS "${BoldCyan}Building Release!${ColorReset}")
    #totally disable log.h
    set(CMAKE_BUILD_TYPE "Release")
    add_definitions(-DQUICK_LOG=6 -Werror=implicit-function-declaration)
endif()

if(PARENTDIR STREQUAL "Debug")
    message(STATUS "${BoldCyan}Building Debug!${ColorReset}")
    set(CMAKE_BUILD_TYPE "Debug")
    #-fno-stack-protector: to debug stack smashing (https://stackoverflow.com/a/1347464/1887602)
    add_definitions(-DQUICK_LOG=6 -DDEBUG -Werror=implicit-function-declaration -fno-stack-protector)
endif(PARENTDIR STREQUAL "Debug")
add_definitions(-Wfatal-errors -std=gnu99)

# ****************** SUB DIRECTORIES *************************
add_subdirectory(src/main/c)
if(${THEPROJECT_TEST_ENABLE_TEST_COMPILATION} STREQUAL "true")
    add_subdirectory(src/test/c)
endif(${THEPROJECT_TEST_ENABLE_TEST_COMPILATION} STREQUAL "true")
