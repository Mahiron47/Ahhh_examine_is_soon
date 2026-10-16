cmake_minimum_required(VERSION 3.16.0)

add_executable(instance-test instance-test.cpp)

target_link_libraries(instance-test PRIVATE lab3-lib
                                            Catch2::Catch2
                                            Catch2::Catch2WithMain)

catch_discover_tests(instance-test TEST_PREFIX "instanceZZ"
                                   TEST_LIST instance-test)