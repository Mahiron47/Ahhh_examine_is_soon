cmake_minimum_required(VERSION 4.4.3)

add_library(static_lib STATIC static.cpp)

target_include_directories(static_lib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})