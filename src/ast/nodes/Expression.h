#ifndef PYTHONCOMPILER_EXPRESSION_H
#define PYTHONCOMPILER_EXPRESSION_H

#include "Node.h"


class Literal : public Expression {};


class Char final : public Literal {
public:
    explicit Char(char value);

    void accept(ASTVisitor* visitor) override { visitor->visit(this); }

    [[nodiscard]] char getValue() const { return value; }

private:
    char value;
};


class Int final : public Literal {
public:
    explicit Int(int value);

    void accept(ASTVisitor* visitor) override { visitor->visit(this); }

    [[nodiscard]] int getValue() const { return value; }

private:
    int value;
};


class Float final : public Literal {
public:
    explicit Float(float value);

    void accept(ASTVisitor* visitor) override { visitor->visit(this); }

    [[nodiscard]] float getValue() const { return value; }

private:
    float value;
};


#endif //PYTHONCOMPILER_EXPRESSION_H
