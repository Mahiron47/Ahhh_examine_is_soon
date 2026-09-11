cmake_minimum_required(VERSION 4.4.3)

add_executable(protocol-test protocol-test.cpp)

target_link_libraries(protocol-test PRIVATE static_lib
                                            dynamic_lib
                                            Catch2::Catch2
                                            Catch2::Catch2WithMain)

catch_discover_tests(protocol-test TEST_PREFIX "protocolZZ" 
                                   TEST_LIST protocol-test)
