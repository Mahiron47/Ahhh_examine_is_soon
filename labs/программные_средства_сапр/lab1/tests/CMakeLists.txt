cmake_minimum_required(VERSION 4.4.3)

include(FetchContent)

FetchContent_Declare(Catch2 GIT_REPOSITORY https://github.com/catchorg/Catch2.git
                            GIT_TAG v3.5.4)

FetchContent_MakeAvailable(Catch2)

include(Catch)

add_subdirectory(matrix-test)
add_subdirectory(algorithm-test)