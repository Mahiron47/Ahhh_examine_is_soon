cmake_minimum_required(VERSION 3.10.0)

# Подключение Catch2
include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.5.0
)

FetchContent_MakeAvailable(Catch2)

# Собираем исходники проекта (исключая main.cpp)
file(GLOB_RECURSE TEST_PROJECT_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/src/Life.cpp"
    "${CMAKE_SOURCE_DIR}/src/Life.hpp"
    "${CMAKE_SOURCE_DIR}/src/*/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/*/*.hpp"
)

# Собираем тестовые исходники
file(GLOB TEST_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

# Создаём исполняемый файл тестов
add_executable(tests ${TEST_SOURCES} ${TEST_PROJECT_SOURCES})

# Линкуем Catch2
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

# Регистрируем тесты для CTest
include(Catch)
catch_discover_tests(tests)
