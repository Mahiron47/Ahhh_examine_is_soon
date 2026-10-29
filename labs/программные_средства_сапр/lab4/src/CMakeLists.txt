cmake_minimum_required(VERSION 3.16.0)

add_library(lab4-lib STATIC app/app.cpp)

target_include_directories(lab4-lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

