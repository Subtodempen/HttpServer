cmake_minimum_required(VERSION 3.5)
project(PrinceServer)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(DEBUG "add -g compiler arg" OFF)

if(DEBUG)
    add_compile_options(-g -Wall -Wextra -Wpedantic)
endif()


file(GLOB_RECURSE SOURCES "src/*.cpp")

include_directories(include)

add_executable(HTTP ${SOURCES})