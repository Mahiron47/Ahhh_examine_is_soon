cmake_minimum_required(VERSION 4.4.3)

add_executable(solution-individual main.cpp)
target_link_libraries(solution-individual PRIVATE dynamic_lib)