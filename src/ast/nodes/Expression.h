#ifndef PYTHONCOMPILER_EXPRESSION_H
#define PYTHONCOMPILER_EXPRESSION_H

#include "Node.h"


class Binary final : public Expression {
public:
    explicit Binary(std::unique_ptr<Expression> lhs, Token operation, std::unique_ptr<Expression> rhs);

    void accept(ASTVisitor* visitor) override { return visitor->visit(this); };

    [[nodiscard]] Expression* getLhs() const { return lhs.get(); }
    [[nodiscard]] Expression* getRhs() const { return rhs.get(); }
    [[nodiscard]] Token& getOperation() { return operation; }

private:
    std::unique_ptr<Expression> lhs;
    std::unique_ptr<Expression> rhs;
    Token operation;
};


class Unary final : public Expression {
public:
    explicit Unary(Token operation, std::unique_ptr<Expression> expr);

    void accept(ASTVisitor* visitor) override { return visitor->visit(this); };

    [[nodiscard]] Expression* getExpr() const { return expr.get(); }
    [[nodiscard]] Token& getOperation() { return operation; }

private:
    std::unique_ptr<Expression> expr;
    Token operation;
};


class Identifier final : public Expression {
public:
    explicit Identifier(std::string name);

    void accept(ASTVisitor* visitor) override { return visitor->visit(this); };

    [[nodiscard]] std::string& getName() { return name; };

private:
    std::string name;
};


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
