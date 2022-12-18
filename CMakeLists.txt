cmake_minimum_required(VERSION 3.24)
project(fileString)

set(CMAKE_CXX_STANDARD 20)

add_executable(fileString main.cpp)

add_executable(C_Test tests/test.c tests/test.txt.h)
