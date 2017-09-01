cmake_minimum_required(VERSION 3.5.1)

message(INFO "You should call cmake when you are in build/Debug or in build/Release. You should call 'cmake ../..'")

set(PROJECT_NAME "CrashC")
project(${PROJECT_NAME} VERSION 1.0)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif(NOT CMAKE_BUILD_TYPE)

add_definitions(-Wfatal-errors)

add_subdirectory(src/main/c)
add_subdirectory(src/test/c)
