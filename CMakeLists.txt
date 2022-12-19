cmake_minimum_required(VERSION 3.2)
project(fileString)

set(CMAKE_CXX_STANDARD 20)

add_executable(fileString main.cpp)

install(TARGETS fileString DESTINATION bin)

add_custom_target(C_Test
        COMMAND fileString tests/test.txt
        COMMAND gcc tests/test.c -o ${PROJECT_BINARY_DIR}/C_Test

        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}

        DEPENDS tests/test.txt tests/test.c
        )

add_custom_target(C++_Test
        COMMAND fileString tests/test.txt -l C++
        COMMAND g++ tests/test.cpp -o ${PROJECT_BINARY_DIR}/C++_Test

        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}

        DEPENDS tests/test.txt tests/test.cpp
        )
