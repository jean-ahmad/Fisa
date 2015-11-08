######################################################################
# Project
######################################################################

cmake_minimum_required(VERSION 2.6)

project(FISA)

set(FISA_VERSION_MAJOR "1")
set(FISA_VERSION_MINOR "0")
set(FISA_VERSION_PATCH "0")

######################################################################
# Options
######################################################################

option(WARNING "When running, the machine will print consistency information." OFF)
if(WARNING)
add_definitions(-D WARNING)
else(WARNING)
add_definitions(-U WARNING)
endif(WARNING)

option(DEBUG "When running, the machine will print debugging information." OFF)
if(DEBUG)
add_definitions(-D DEBUG)
else(DEBUG)
add_definitions(-U DEBUG)
endif(DEBUG)

include(CheckFunctionExists)
check_function_exists(gettimeofday GCCTIME)
if(GCCTIME)
add_definitions(-D GCCTIME_SUPPORT)
else(GCCTIME)
add_definitions(-U GCCTIME_SUPPORT)
endif(GCCTIME)

######################################################################
# Building
######################################################################

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ansi -Wpedantic -std=c++11 ")

include_directories ("${PROJECT_SOURCE_DIR}/src")

add_subdirectory("${PROJECT_SOURCE_DIR}/src")
add_subdirectory("${PROJECT_SOURCE_DIR}/examples")

######################################################################
# Installation
######################################################################

file(GLOB HEADERS "src/*.hpp")
install(FILES ${HEADERS} DESTINATION "include")

