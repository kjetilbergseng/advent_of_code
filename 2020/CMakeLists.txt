cmake_minimum_required(VERSION "3.13.2")

project("Advent_of_code")

include_directories(include)
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
    "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(warnings "-Wall -Wextra -Werror")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(warnings "/W4 /WX /EHsc")
endif()

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} ${warnings}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${warnings}")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MD")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(
    "${PROJECT_NAME}"
    "src/main.cpp"
    "src/day1.cpp"
    "src/day1.h"
    "src/day2.cpp"
    "src/day2.h"
    "src/day3.cpp"
    "src/day3.h"
    "src/day4.cpp"
    "src/day4.h"
    "src/day5.cpp"
    "src/day5.h"
    "src/day6.cpp"
    "src/day6.h"
    "src/day7.cpp"
    "src/day7.h"
    "src/day8.cpp"
    "src/day8.h"
    "src/day9.cpp"
    "src/day9.h"
    "src/day10.cpp"
    "src/day10.h"
    "src/day11.cpp"
    "src/day11.h"
    "src/day12.cpp"
    "src/day12.h"
    "src/day13.cpp"
    "src/day13.h"
    "src/day14.cpp"
    "src/day14.h"
    "src/utilities.h"
    )
target_link_libraries("${PROJECT_NAME}" ${CONAN_LIBS})
