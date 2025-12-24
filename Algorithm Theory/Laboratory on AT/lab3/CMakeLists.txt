cmake_minimum_required(VERSION 3.10.0)
project(lab3TA VERSION 0.1.0 LANGUAGES C CXX)

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/*.hpp"
    "${CMAKE_SOURCE_DIR}/src/*/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/*/*.hpp"
)

add_executable(lab3TA ${PROJECT_SOURCES})

# Включаем CTest
enable_testing()

# Подключение тестов
add_subdirectory(tests)
