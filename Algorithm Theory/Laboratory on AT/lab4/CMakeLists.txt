cmake_minimum_required(VERSION 3.10.0)
project(lab4 VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/*.hpp"
    "${CMAKE_SOURCE_DIR}/src/*/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/*/*.hpp"
)

add_executable(lab4 ${PROJECT_SOURCES})

enable_testing()

add_subdirectory(tests)