#ifndef PYTHONCOMPILER_STATEMENT_H
#define PYTHONCOMPILER_STATEMENT_H

#include <memory>
#include <vector>

#include "../../common/Token.h"
#include "Node.h"
#include "Type.h"


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
    explicit Assignment(Token identifier, std::unique_ptr<Type> type, std::unique_ptr<Expression> expr);

    void accept(ASTVisitor* visitor) override { visitor->visit(this); }

    [[nodiscard]] Token& getIdentifier() { return identifier; }
    [[nodiscard]] Type* getType() const { return type.get(); }
    [[nodiscard]] Expression* getExpr() const { return expr.get(); }

private:
    Token identifier;
    std::unique_ptr<Type> type;
    std::unique_ptr<Expression> expr;
};


class Break final : public Statement {
public:
    Break() = default;

    void accept(ASTVisitor* visitor) override { visitor->visit(this); }
};


class Continue final : public Statement {
public:
    Continue() = default;

    void accept(ASTVisitor* visitor) override { visitor->visit(this); }
};


class Discard final : public Statement {
public:
    explicit Discard(std::unique_ptr<Expression> expr);

    void accept(ASTVisitor* visitor) override { visitor->visit(this); }

    [[nodiscard]] Expression* getExpr() const { return expr.get(); }

private:
    std::unique_ptr<Expression> expr;
};


class ForEach final : public Statement {
public:
    explicit ForEach(Token identifier, std::unique_ptr<Expression> iterable, std::unique_ptr<Scope> bodyScope, std::unique_ptr<Scope> elseScope);

    void accept(ASTVisitor* visitor) override { visitor->visit(this); }

    [[nodiscard]] Token& getIdentifier() { return identifier; }
    [[nodiscard]] Expression* getIterable() const { return iterable.get(); }
    [[nodiscard]] Scope* getBodyScope() const { return bodyScope.get(); }
    [[nodiscard]] Scope* getElseScope() const { return elseScope.get(); }

private:
    Token identifier;
    std::unique_ptr<Expression> iterable;
    std::unique_ptr<Scope> bodyScope;
    std::unique_ptr<Scope> elseScope;
};


class Function final : public Statement {
public:
    explicit Function(
        Token name,
        std::vector<std::unique_ptr<TypeParameter>> typeParameters,
        std::unique_ptr<Type> returnType,
        std::vector<std::unique_ptr<Parameter>> parameters,
        std::unique_ptr<Scope> body
    );

    void accept(ASTVisitor* visitor) override { visitor->visit(this); }

    [[nodiscard]] Token& getName() { return name; }
    [[nodiscard]] std::vector<std::unique_ptr<TypeParameter>>& getTypeParameters() { return typeParameters; }
    [[nodiscard]] Type* getReturnType() const { return returnType.get(); }
    [[nodiscard]] std::vector<std::unique_ptr<Parameter>>& getParameters() { return parameters; }
    [[nodiscard]] Scope* getBody() const { return body.get(); }

private:
    Token name;
    std::vector<std::unique_ptr<TypeParameter>> typeParameters;
    std::unique_ptr<Type> returnType;
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


class While final : public Statement {
public:
    explicit While(std::unique_ptr<Expression> condition, std::unique_ptr<Scope> bodyScope, std::unique_ptr<Scope> elseScope);

    void accept(ASTVisitor* visitor) override { visitor->visit(this); }

    [[nodiscard]] Expression* getCondition() const { return condition.get(); }
    [[nodiscard]] Scope* getBodyScope() const { return bodyScope.get(); }
    [[nodiscard]] Scope* getElseScope() const { return elseScope.get(); }

private:
    std::unique_ptr<Expression> condition;
    std::unique_ptr<Scope> bodyScope;
    std::unique_ptr<Scope> elseScope;
};

#endif //PYTHONCOMPILER_STATEMENT_H
