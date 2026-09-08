cmake_minimum_required(VERSION 4.4.3)

add_executable(task1_static ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
target_link_libraries(task1_static PRIVATE static_lib)

add_executable(task1_dynamic ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
target_link_libraries(task1_dynamic PRIVATE dynamic_lib)