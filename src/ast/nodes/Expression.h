#ifndef PYTHONCOMPILER_EXPRESSION_H
#define PYTHONCOMPILER_EXPRESSION_H

#include "Node.h"


class Expression : public Node {
public:
    Expression() = default;
};


class Literal : public Expression {};


class Char : public Literal {
public:
    explicit Char(char value);

private:
    char value;
};


class Int : public Literal {
public:
    explicit Int(int value);

private:
    int value;
};


#endif //PYTHONCOMPILER_EXPRESSION_H
