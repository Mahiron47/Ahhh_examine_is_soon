cmake_minimum_required(VERSION 4.4.3)

set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

add_library(dynamic_lib STATIC dynamic.cpp)

target_include_directories(dynamic_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
