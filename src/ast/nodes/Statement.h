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

    void addStatement(std::unique_ptr<Statement> statement) { this->statements.push_back(std::move(statement)); }

private:
    std::vector<std::unique_ptr<Statement>> statements;
};


class Comment : public Statement {
public:
    Comment(std::string  comment);

private:
    std::string comment;
};


class Declaration : public Statement {
public:
    Declaration(Token  identifier, Token  type, std::unique_ptr<Expression> expr);

private:
    Token identifier;
    Token type;
    std::unique_ptr<Expression> expr;
};


class Function : public Statement {
public:
    Function() = default;
};


#endif //PYTHONCOMPILER_STATEMENT_H
