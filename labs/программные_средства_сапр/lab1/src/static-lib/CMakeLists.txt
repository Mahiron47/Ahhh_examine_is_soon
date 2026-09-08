cmake_minimum_required(VERSION 4.4.3)

add_library(static_lib INTERFACE)

target_include_directories(static_lib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})