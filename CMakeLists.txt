cmake_minimum_required(VERSION 4.0.2)
project(PythonCompiler)

set(CMAKE_CXX_STANDARD 26)

add_executable(PythonCompiler main.cpp
        src/common/Token.h
        src/lexer/Lexer.cpp
        src/parser/Parser.cpp
        src/ast/nodes/Node.cpp
        src/ast/nodes/Statement.cpp
        src/ast/nodes/Expression.cpp
        src/ast/ASTRenderer.cpp
        src/symbol/SymbolTable.cpp
        src/ast/nodes/Type.cpp
)
