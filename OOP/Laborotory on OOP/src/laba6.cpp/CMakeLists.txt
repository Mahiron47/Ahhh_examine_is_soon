cmake_minimum_required(VERSION 3.10.0)
project(laba6 VERSION 0.1.0 LANGUAGES C CXX)

file(GLOB_RECURSE SRC_FILES
	"${CMAKE_SOURCE_DIR}/src/*.cpp"
	"${CMAKE_SOURCE_DIR}/src/*/*.cpp"
)

add_executable(laba6 ${CMAKE_SOURCE_DIR}/src/main.cpp ${SRC_FILES})

