cmake_minimum_required(VERSION 4.4.3)

add_executable(matrix-test matrix-test.cpp)

target_link_libraries(matrix-test PRIVATE Catch2::Catch2
                                          Catch2::Catch2WithMain)

catch_discover_tests(matrix-test TEST_PREFIX "matrixZZ" 
                                 WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})