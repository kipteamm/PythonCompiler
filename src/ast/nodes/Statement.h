#ifndef PYTHONCOMPILER_STATEMENT_H
#define PYTHONCOMPILER_STATEMENT_H

#include <memory>
#include <vector>

#include "../../common/Token.h"
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
    explicit Assignment(Token identifier, Token type, std::unique_ptr<Expression> expr);

    void accept(ASTVisitor* visitor) override { visitor->visit(this); }

    [[nodiscard]] Token& getIdentifier() { return identifier; }
    [[nodiscard]] Token& getType() { return type; }
    [[nodiscard]] Expression* getExpr() const { return expr.get(); }

private:
    Token identifier;
    Token type;
    std::unique_ptr<Expression> expr;
};


class Discard final : public Statement {
public:
    explicit Discard(std::unique_ptr<Expression> expr);

    void accept(ASTVisitor* visitor) override { visitor->visit(this); }

    [[nodiscard]] Expression* getExpr() const { return expr.get(); }

private:
    std::unique_ptr<Expression> expr;
};


class Function final : public Statement {
public:
    explicit Function(Token name, Token returnType, std::vector<std::unique_ptr<Parameter>> parameters, std::unique_ptr<Scope> body);

    void accept(ASTVisitor* visitor) override { visitor->visit(this); }

    [[nodiscard]] Token& getName() { return name; }
    [[nodiscard]] Token& getReturnType() { return returnType; }
    [[nodiscard]] std::vector<std::unique_ptr<Parameter>>& getParameters() { return parameters; }
    [[nodiscard]] Scope* getBody() const { return body.get(); }

private:
    Token name;
    Token returnType;
    std::vector<std::unique_ptr<Parameter>> parameters;
    std::unique_ptr<Scope> body;
};


class If final : public Statement {
public:
    explicit If(std::unique_ptr<Expression> condition, std::unique_ptr<Scope> thenScope, std::unique_ptr<Scope> elseScope);

    void accept(ASTVisitor* visitor) override { visitor->visit(this); }

    [[nodiscard]] Expression* getCondition() const { return condition.get(); }
    [[nodiscard]] Scope* getThenScope() const { return thenScope.get(); }
    [[nodiscard]] Scope* getElseScope() const { return elseScope.get(); }

private:
    std::unique_ptr<Expression> condition;
    std::unique_ptr<Scope> thenScope;
    std::unique_ptr<Scope> elseScope;
};


class Return final : public Statement {
public:
    explicit Return(std::unique_ptr<Expression> expr);

    void accept(ASTVisitor* visitor) override { visitor->visit(this); }

    [[nodiscard]] Expression* getExpr() const { return expr.get(); }

private:
    std::unique_ptr<Expression> expr;
};

#endif //PYTHONCOMPILER_STATEMENT_H
