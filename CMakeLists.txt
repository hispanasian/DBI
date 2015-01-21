cmake_minimum_required(VERSION 2.8.4)
project(DBI)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
add_library(BASE_LIB src/Schema.cc
                     src/Record.cc
                     src/Comparison.cc
                     src/ComparisonEngine.cc
                     src/y.tab.c)
set(SOURCE_FILES src/main.cc ${BASE_LIB})
add_executable(DBI ${SOURCE_FILES})

################################
# GTest
################################
set(gtest_SOURCE_DIR libs/gtest-1.7.0)
add_subdirectory (libs/gtest-1.7.0)
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

################################
# Unit Tests
################################
# Add sample cpp file
#add_executable( runUnitTests test/simpletest.cpp )
# Link sample executable against gtest & gtest_main
#target_link_libraries(UnitTests gtest gtest_main)
#add_test( UnitTests runUnitTests )