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


class Scope : public Statement {
public:
    Scope() = default;

    void accept(ASTVisitor* visitor) override { visitor->visit(this); }

    void addStatement(std::unique_ptr<Statement> statement) { this->statements.push_back(std::move(statement)); }
    [[nodiscard]] const std::vector<std::unique_ptr<Statement>>& getStatements() const { return statements; }

private:
    std::vector<std::unique_ptr<Statement>> statements;
};


class Comment : public Statement {
public:
    explicit Comment(std::string  comment);

    void accept(ASTVisitor* visitor) override { visitor->visit(this); }

    std::string& getComment() { return comment; }

private:
    std::string comment;
};


class Assignment : public Statement {
public:
    Assignment(Token identifier, Token  type, std::unique_ptr<Expression> expr);

    void accept(ASTVisitor* visitor) override { visitor->visit(this); }

    [[nodiscard]] Token& getIdentifier() { return identifier; }
    [[nodiscard]] Token& getType() { return type; }
    [[nodiscard]] Expression* getExpr() const { return expr.get(); }

private:
    Token identifier;
    Token type;
    std::unique_ptr<Expression> expr;
};


class Function : public Statement {
public:
    Function() = default;

    void accept(ASTVisitor* visitor) override { visitor->visit(this); }
};


#endif //PYTHONCOMPILER_STATEMENT_H
