cmake_minimum_required(VERSION 3.16)

project(lab3 VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra -Wpedantic -Werror")

add_subdirectory(src)
add_subdirectory(instance)

enable_testing()

add_subdirectory(test)
