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
        src/ast/ASTVisitor.cpp
        src/ast/DotVisitor.cpp)

enable_testing()

file(GLOB_RECURSE TEST_FILES "tests/*.py")

foreach(TEST_FILE ${TEST_FILES})
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)
    get_filename_component(TEST_DIR ${TEST_FILE} DIRECTORY)
    get_filename_component(CATEGORY ${TEST_DIR} NAME)

    add_test(
            NAME "test_${CATEGORY}_${TEST_NAME}"
            COMMAND ${CMAKE_COMMAND} -E env python3
            ${CMAKE_CURRENT_SOURCE_DIR}/tests/run_test.py
            $<TARGET_FILE:PythonCompiler>
            ${TEST_FILE}
    )
endforeach()
