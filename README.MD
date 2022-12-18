## FileString - Commandline Text to Header file converter

You want to include your help page in your binary? 
Or any text in a Microcontroller Program? 
No you can without putting all the escape sequences there yourself.
FileString converts any textfile to a C or C++ Header file with the original text as a
string with all special characters escaped in it.

To use these Headers, just include them and use the string 
(name = filename without extension) like you normally would. See `tests` for examples.

## Getting started

### 0. Install CMake and G++

You probably already have them if you need this

### 1. Clone this repo

    git clone https://github.com/TjarkG/fileString
    cd fileString

### 2. Compile & Install

    mkdir cmake-build-debug  && cd $_
    cmake ..
    sudo make install

### 3. Start Converting / usage with CMake and make

To see if the installation was successful, 
go to the `tests` directory and generate `test.txt.h` with

    fileString test.txt -l C++

CMake:

    add_custom_target(C_Test
        COMMAND fileString tests/test.txt
        COMMAND gcc tests/test.c -o ${PROJECT_BINARY_DIR}/C_Test
    
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    
        DEPENDS tests/test.txt tests/test.c tests/test.txt.h
    )

make:

    src/example.h: example
        fileString example -o src

## Command Line Arguments
How to use: `$ filestring [Options] [Input Files]`

Options:
- -h Show help message
- -v Verbose output, prints output paths
- -l Language, followed by language specifier (C, C++ or cpp), default is C
- -o Output directory (default: equal to Input Directory)