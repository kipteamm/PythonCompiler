#ifndef PYTHONCOMPILER_EXPRESSION_H
#define PYTHONCOMPILER_EXPRESSION_H

#include <vector>

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


class Dictionary final : public Expression {
public:
    Dictionary();
    explicit Dictionary(std::vector<std::unique_ptr<Expression>> keys, std::vector<std::unique_ptr<Expression>> values);

    void accept(ASTVisitor* visitor) override { return visitor->visit(this); };

    [[nodiscard]] std::vector<std::unique_ptr<Expression>>& getKeys() { return keys; }
    [[nodiscard]] std::vector<std::unique_ptr<Expression>>& getValues() { return values; }

private:
    std::vector<std::unique_ptr<Expression>> keys;
    std::vector<std::unique_ptr<Expression>> values;
};


class FunctionCall final : public Expression {
public:
    explicit FunctionCall(Token identifier, std::vector<std::unique_ptr<Expression>> arguments);

    void accept(ASTVisitor* visitor) override { return visitor->visit(this); };

    [[nodiscard]] Token& getIdentifier() { return identifier; }
    [[nodiscard]] std::vector<std::unique_ptr<Expression>>& getArguments() { return arguments; }

private:
    Token identifier;
    std::vector<std::unique_ptr<Expression>> arguments;
};


class Identifier final : public Expression {
public:
    explicit Identifier(std::string name);

    void accept(ASTVisitor* visitor) override { return visitor->visit(this); };

    [[nodiscard]] std::string& getName() { return name; };

private:
    std::string name;
};


class List final : public Expression {
public:
    List();
    explicit List(std::vector<std::unique_ptr<Expression>> values);

    void accept(ASTVisitor* visitor) override { return visitor->visit(this); };

    [[nodiscard]] std::vector<std::unique_ptr<Expression>>& getValues() { return values; }

private:
    std::vector<std::unique_ptr<Expression>> values;
};


class Literal : public Expression {};


class Bool final : public Literal {
public:
    explicit Bool(bool value);

    void accept(ASTVisitor* visitor) override { visitor->visit(this); }

    [[nodiscard]] bool getValue() const { return value; }

private:
    bool value;
};


class Char final : public Literal {
public:
    explicit Char(char value);

    void accept(ASTVisitor* visitor) override { visitor->visit(this); }

    [[nodiscard]] char getValue() const { return value; }

private:
    char value;
};


class Float final : public Literal {
public:
    explicit Float(float value);

    void accept(ASTVisitor* visitor) override { visitor->visit(this); }

    [[nodiscard]] float getValue() const { return value; }

private:
    float value;
};


class Int final : public Literal {
public:
    explicit Int(int value);

    void accept(ASTVisitor* visitor) override { visitor->visit(this); }

    [[nodiscard]] int getValue() const { return value; }

private:
    int value;
};


class String final : public Literal {
public:
    explicit String(std::string value);

    void accept(ASTVisitor* visitor) override { visitor->visit(this); }

    [[nodiscard]] std::string getValue() const { return value; }

private:
    std::string value;
};


class JoinedString final : public Literal {
public:
    explicit JoinedString(std::vector<std::unique_ptr<Expression>> values);

    void accept(ASTVisitor* visitor) override { visitor->visit(this); }

    [[nodiscard]] std::vector<std::unique_ptr<Expression>>& getValues() { return values; }

private:
    std::vector<std::unique_ptr<Expression>> values;
};


#endif //PYTHONCOMPILER_EXPRESSION_H
