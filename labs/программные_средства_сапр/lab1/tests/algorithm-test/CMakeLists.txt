cmake_minimum_required(VERSION 4.4.3)

add_executable(algorithm-test wave_algorithm-test.cpp)

target_link_libraries(algorithm-test PRIVATE static_lib
                                             dynamic_lib
                                             Catch2::Catch2
                                             Catch2::Catch2WithMain)

catch_discover_tests(algorithm-test TEST_PREFIX "algorithmZZ"
                                    TEST_LIST algorithm-test)