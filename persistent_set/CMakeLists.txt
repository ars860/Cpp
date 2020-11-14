cmake_minimum_required(VERSION 3.9.6)
project(persistent_set)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

include_directories(${persistent_set_SOURCE_DIR})

add_executable(persistent_set persistent_set.h main.cpp)

add_executable(testing
        testing.cpp
        persistent_set.h
        gtest/gtest-all.cc
        gtest/gtest.h
        gtest/gtest_main.cc)


target_link_libraries(testing -lgmp -lgmpxx -lpthread)


