cmake_minimum_required(VERSION 4.4.3)
project(lab1 VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra -Wpedantic -Werror")

enable_testing()

add_subdirectory(src/static-lib)
add_subdirectory(src/dynamic-lib)
add_subdirectory(src/solution-minimum/task1)
add_subdirectory(src/solution-minimum/task2)
add_subdirectory(src/solution-minimum/task3)
add_subdirectory(src/solution-maximum)
add_subdirectory(tests)