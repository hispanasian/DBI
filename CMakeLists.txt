cmake_minimum_required(VERSION 2.8.4)
project(DBI)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Prepping bison and flex dependent files
find_package(BISON)
find_package(FLEX)
BISON_TARGET(MyParser src/Parser.y ${CMAKE_CURRENT_BINARY_DIR}/src/y.tab.c)
FLEX_TARGET(MyScanner src/Lexer.l ${CMAKE_CURRENT_BINARY_DIR}/src/lex.yy.c)
ADD_FLEX_BISON_DEPENDENCY(MyScanner MyParser)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
set(BISON_FLEX_LIB ${BISON_MyParser_OUTPUTS} ${FLEX_MyScanner_OUTPUTS})

# The remaining code
add_library(BASE_LIB src/Schema.cc
                     src/Record.cc
                     src/Comparison.cc
                     src/ComparisonEngine.cc
                     src/y.tab.c
                     ${BISON_FLEX_LIB})
# Disable linking
set_target_properties(${BASE_LIB} PROPERTIES COMPILE_FLAGS -c)

set(SOURCE_FILES src/main.cc ${BASE_LIB})
add_executable(DBI ${SOURCE_FILES})

################################
# GTest
################################
set(gtest_SOURCE_DIR lib/gtest-1.7.0)
add_subdirectory (lib/gtest-1.7.0)
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