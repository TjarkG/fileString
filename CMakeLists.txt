cmake_minimum_required(VERSION 3.24)
project(fileString)

set(CMAKE_CXX_STANDARD 20)

add_executable(fileString main.cpp)

install(TARGETS fileString DESTINATION bin)

add_executable(C_Test tests/test.c tests/test.txt.h)

add_executable(C++_Test tests/test.cpp tests/test.txt.hpp)
