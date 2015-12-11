######################################################################
# Project
######################################################################

cmake_minimum_required(VERSION 3.1)

project(FISA)

set(FISA_VERSION_MAJOR "1")
set(FISA_VERSION_MINOR "0")
set(FISA_VERSION_PATCH "0")

######################################################################
# Options
######################################################################

option(WARNING "When running, the machine will print consistency information." OFF)
if(WARNING)
add_definitions(-DWARNING)
else(WARNING)
add_definitions(-UWARNING)
endif(WARNING)

option(DEBUG "When running, the machine will print debugging information." OFF)
if(DEBUG)
add_definitions(-DDEBUG)
else(DEBUG)
add_definitions(-UDEBUG)
endif(DEBUG)

set(NON_REGRESSION_TESTS OFF CACHE BOOL "Building non-regression tests.")

######################################################################
# Configuration
######################################################################

include(CheckFunctionExists)
check_function_exists(gettimeofday GETTIMEOFDAY)
if(GETTIMEOFDAY)
add_definitions(-DOPENSOURCE_PLATFORM_TIME)
else(GETTIMEOFDAY)
add_definitions(-UOPENSOURCE_PLATFORM_TIME)
endif(GETTIMEOFDAY)

include(CheckLibraryExists)
check_library_exists(Kernel32 GetSystemTime "windows.h" GETSYSTEMTIME)
if(GETSYSTEMTIME)
add_definitions(-DWINDOWS_PLATFORM_TIME)
else(GETSYSTEMTIME)
add_definitions(-UWINDOWS_PLATFORM_TIME)
endif(GETSYSTEMTIME)

cmake_policy(SET CMP0054 NEW)
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ansi -Wpedantic -std=c++11 ")
endif()

######################################################################
# Building
######################################################################

include_directories ("${PROJECT_SOURCE_DIR}/src")

add_subdirectory("${PROJECT_SOURCE_DIR}/src")
add_subdirectory("${PROJECT_SOURCE_DIR}/examples")
add_subdirectory("${PROJECT_SOURCE_DIR}/tests")

######################################################################
# Installation
######################################################################

file(GLOB HEADERS "src/*.hpp")
install(FILES ${HEADERS} DESTINATION "include")
