cmake_minimum_required(VERSION 4.2)
project(PythonCompiler)

set(CMAKE_CXX_STANDARD 26)

add_executable(PythonCompiler main.cpp
        src/Lexer.cpp
        src/Token.h)
