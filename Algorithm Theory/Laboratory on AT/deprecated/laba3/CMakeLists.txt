cmake_minimum_required(VERSION 3.10)
project(BrainfuckInterpretator VERSION 0.1.0 LANGUAGES C CXX)

# Require at least C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Optionally set default build type for single-config generators
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type" FORCE)
endif()

# Sources
set(SOURCES
  BrainfuckInterpretator.cpp
  Logger.cpp
  main.cpp
  Memory.cpp
  Pointer.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

# Ensure headers in source directory are found during out-of-tree builds
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Compiler warnings (GCC/Clang / MSVC conditional)
if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /W4 /permissive-)
else()
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Use modern CMake guarantees for C++ features when needed
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

# Option to enable debug logging at compile time. Can be set via
# `-DENABLE_DEBUG_LOGGING=ON` or (for backward compatibility) `-DDEBUG=ON`.
option(DEBUG OFF)
if(DEBUG)
  target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG)
  message(STATUS "Debug logging enabled")
endif()
