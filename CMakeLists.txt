cmake_minimum_required(VERSION 3.10)

project(test_pr)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Глобально собираем все исходники и заголовочные файлы
file(GLOB SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp)
file(GLOB HEADERS ${CMAKE_SOURCE_DIR}/include/*.hpp)

# Поиск SQLite3
find_package(SQLite3 REQUIRED)

# Создание исполнимого файла
add_executable(test_pr ${SOURCES} ${HEADERS})

# Указываем директорию с заголовочными файлами
target_include_directories(test_pr PRIVATE include)

# Линковка с SQLite3
target_link_libraries(test_pr PRIVATE SQLite::SQLite3)
