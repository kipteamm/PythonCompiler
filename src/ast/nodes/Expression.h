#ifndef PYTHONCOMPILER_EXPRESSION_H
#define PYTHONCOMPILER_EXPRESSION_H

#include "Node.h"


class Expression : public Node {
public:
    Expression() = default;

    void accept(ASTVisitor *visitor) override;
};


class Literal : public Expression {};


class Char : public Literal {
public:
    explicit Char(char value);

    void accept(ASTVisitor* visitor) override { visitor->visit(this); }

    [[nodiscard]] char getValue() const { return value; }

private:
    char value;
};


class Int : public Literal {
public:
    explicit Int(int value);

    void accept(ASTVisitor* visitor) override { visitor->visit(this); }

    [[nodiscard]] int getValue() const { return value; }

private:
    int value;
};


#endif //PYTHONCOMPILER_EXPRESSION_H
