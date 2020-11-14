cmake_minimum_required(VERSION 3.9)
project(bigint)

#set(CMAKE_SYSTEM_NAME Ubuntu)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_RELEASE "-O")

#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,undefined -D_GLIBCXX_DEBUG")

include_directories(${bigint_SOURCE_DIR})

add_executable(big_integer_testing
        big_integer_testing.cpp
        big_integer.h
        big_integer.cpp
        victor.cpp
        victor.h
        gtest/gtest-all.cc
        gtest/gtest.h
        gtest/gtest_main.cc)

target_link_libraries(big_integer_testing -lgmp -lgmpxx -lpthread)

add_executable(victor_tesing victor_testing.cpp
        victor.h
        victor.cpp)

add_executable(my_test big_integer.h
        big_integer.cpp
        main.cpp)


