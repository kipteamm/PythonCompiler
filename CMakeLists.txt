cmake_minimum_required(VERSION 4.2)
project(PythonCompiler)

set(CMAKE_CXX_STANDARD 26)

add_executable(PythonCompiler main.cpp
        src/common/Token.h
        src/lexer/Lexer.cpp
        src/parsing/Parser.cpp
        src/ast/nodes/Node.cpp
        src/ast/nodes/Statement.cpp
        src/ast/nodes/Expression.cpp)
