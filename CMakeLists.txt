cmake_minimum_required(VERSION 2.8.4)
project(DBI)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES src/main.cpp)
add_executable(DBI ${SOURCE_FILES})