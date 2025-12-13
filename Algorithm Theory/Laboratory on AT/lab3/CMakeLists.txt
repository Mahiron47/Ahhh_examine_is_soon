cmake_minimum_required(VERSION 3.10.0)
project(lab3TA VERSION 0.1.0 LANGUAGES C CXX)

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/*.hpp"
    "${CMAKE_SOURCE_DIR}/src/*/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/*/*.hpp"
)

add_executable(lab3TA ${CMAKE_SOURCE_DIR}/src/main.cpp)

# Установить аргументы командной строки по умолчанию
set(PROGRAM_ARGS "-DEBUG" "-i" "${CMAKE_SOURCE_DIR}/input.txt" CACHE STRING "Arguments to pass to the program")

# Добавить custom target для запуска с аргументами
add_custom_target(run
    COMMAND lab3TA ${PROGRAM_ARGS}
    DEPENDS lab3TA
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running lab3TA with arguments: ${PROGRAM_ARGS}"
)
