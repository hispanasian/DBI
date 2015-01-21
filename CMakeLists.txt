cmake_minimum_required(VERSION 2.8.4)
project(DBI)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES src/main.cpp)
add_executable(DBI ${SOURCE_FILES})

################################
# GTest
################################
#set(gtest_SOURCE_DIR libs/gtest-1.7.0)
ADD_SUBDIRECTORY (libs/gtest-1.7.0)
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

################################
# Unit Tests
################################
# Add sample cpp file
#add_executable( runUnitTests test/simpletest.cpp )
# Link sample executable against gtest & gtest_main
target_link_libraries(UnitTests gtest gtest_main)
add_test( UnitTests runUnitTests )