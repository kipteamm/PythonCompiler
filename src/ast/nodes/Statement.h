#ifndef PYTHONCOMPILER_STATEMENT_H
#define PYTHONCOMPILER_STATEMENT_H

#include <memory>
#include <vector>

#include "../../common/Token.h"
#include "Expression.h"
#include "Node.h"


class Statement : public Node {
public:
    Statement() = default;
};


class Scope final : public Statement {
public:
    Scope() = default;

    void accept(ASTVisitor* visitor) override { visitor->visit(this); }

    void addStatement(std::unique_ptr<Statement> statement) { this->statements.push_back(std::move(statement)); }
    [[nodiscard]] const std::vector<std::unique_ptr<Statement>>& getStatements() const { return statements; }

private:
    std::vector<std::unique_ptr<Statement>> statements;
};


class Comment final : public Statement {
public:
    explicit Comment(std::string  comment);

    void accept(ASTVisitor* visitor) override { visitor->visit(this); }

    std::string& getComment() { return comment; }

private:
    std::string comment;
};


class Assignment final : public Statement {
public:
    Assignment(Token identifier, Token type, std::unique_ptr<Expression> expr);

    void accept(ASTVisitor* visitor) override { visitor->visit(this); }

    [[nodiscard]] Token& getIdentifier() { return identifier; }
    [[nodiscard]] Token& getType() { return type; }
    [[nodiscard]] Expression* getExpr() const { return expr.get(); }

private:
    Token identifier;
    Token type;
    std::unique_ptr<Expression> expr;
};


class Function final : public Statement {
public:
    explicit Function(Token name, Token returnType, std::vector<std::unique_ptr<Parameter>> parameters);

    void accept(ASTVisitor* visitor) override { visitor->visit(this); }

    [[nodiscard]] Token& getName() { return name; }
    [[nodiscard]] Token& getReturnType() { return returnType; }
    [[nodiscard]] std::vector<std::unique_ptr<Parameter>>& getParameters() { return parameters; }

private:
    Token name;
    Token returnType;
    std::vector<std::unique_ptr<Parameter>> parameters;
};


#endif //PYTHONCOMPILER_STATEMENT_H
