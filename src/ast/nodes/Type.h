#ifndef PYTHONCOMPILER_TYPE_H
#define PYTHONCOMPILER_TYPE_H

#include <memory>
#include <vector>

#include "../../common/Token.h"
#include "Node.h"


class GenericType final : public Type {
public:
    explicit GenericType(Token base);
    explicit GenericType(Token base, std::vector<std::unique_ptr<Type>> arguments);

    void accept(ASTVisitor* visitor) override { visitor->visit(this); }

    [[nodiscard]] Token& getBase() { return base; }
    [[nodiscard]] const std::vector<std::unique_ptr<Type>>& getArguments() const { return arguments; }

private:
    Token base;
    std::vector<std::unique_ptr<Type>> arguments;
};


class PrimitiveType final : public Type {
public:
    explicit PrimitiveType(Token type);

    void accept(ASTVisitor* visitor) override { visitor->visit(this); }

    [[nodiscard]] Token& getToken() { return type; }

private:
    Token type;
};


class TypeParameter final : public Node {
public:
    explicit TypeParameter(Token identifier, std::unique_ptr<Type> bound = nullptr);

    void accept(ASTVisitor* visitor) override { visitor->visit(this); }

    [[nodiscard]] Token& getIdentifier() { return identifier; }
    [[nodiscard]] Type* getBound() const { return bound.get(); }
    [[nodiscard]] bool hasBound() const { return bound != nullptr; }

private:
    Token identifier;
    std::unique_ptr<Type> bound;
};


class UnionType final : public Type {
public:
    explicit UnionType(std::vector<std::unique_ptr<Type>> types);

    void accept(ASTVisitor* visitor) override { visitor->visit(this); }

    [[nodiscard]] const std::vector<std::unique_ptr<Type>>& getTypes() const { return types; }

private:
    std::vector<std::unique_ptr<Type>> types;
};


class UnresolvedType final : public Type {
public:
    explicit UnresolvedType(Token identifier);

    void accept(ASTVisitor* visitor) override { visitor->visit(this); }

    [[nodiscard]] Token& getIdentifier() { return identifier; }

private:
    Token identifier;
};

#endif //PYTHONCOMPILER_TYPE_H