cmake_minimum_required(VERSION 3.16.0)

set(LOCAL_CATCH2_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../../build/_deps/catch2-src")
if (EXISTS "${LOCAL_CATCH2_DIR}/CMakeLists.txt")
    add_subdirectory("${LOCAL_CATCH2_DIR}" catch2_build EXCLUDE_FROM_ALL)
    list(APPEND CMAKE_MODULE_PATH "${LOCAL_CATCH2_DIR}/extras")
else()
    include(FetchContent)
    FetchContent_Declare(Catch2 GIT_REPOSITORY https://github.com/catchorg/Catch2.git
                                GIT_TAG v3.5.4)
    FetchContent_MakeAvailable(Catch2)
endif()

include(Catch)

add_subdirectory(instance)

