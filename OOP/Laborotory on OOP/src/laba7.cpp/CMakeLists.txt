cmake_minimum_required(VERSION 3.10.0)
project(laba7 VERSION 0.1.0 LANGUAGES C CXX)

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Сбор всех исходных файлов
file(GLOB_RECURSE SRC_FILES
	"${CMAKE_SOURCE_DIR}/src/*.cpp"
)

# Создание исполняемого файла
add_executable(laba7 ${SRC_FILES})

# Добавление директорий для поиска заголовочных файлов
target_include_directories(laba7 PRIVATE "${CMAKE_SOURCE_DIR}/src")
