cmake_minimum_required(VERSION 3.10)

project(SQLApp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)


file(GLOB SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp)
file(GLOB HEADERS ${CMAKE_SOURCE_DIR}/include/*.hpp)
find_package(SQLite3 REQUIRED)

add_executable(SQLApp ${SOURCES} ${HEADERS})

target_include_directories(SQLApp PRIVATE include)
target_link_libraries(SQLApp PRIVATE SQLite::SQLite3)
target_compile_options(SQLApp PRIVATE -Wall -Werror -Wextra)