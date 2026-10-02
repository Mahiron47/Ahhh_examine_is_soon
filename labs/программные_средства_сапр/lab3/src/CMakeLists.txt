cmake_minimum_required(VERSION 3.16.0)

add_library(lab3-lib STATIC app/app.cpp)

target_include_directories(lab3-lib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})