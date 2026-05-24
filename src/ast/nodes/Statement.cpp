#include "Statement.h"

#include <utility>


Comment::Comment(std::string comment)
    : comment(std::move(comment)) {}


Assignment::Assignment(Token identifier, Token type, std::unique_ptr<Expression> expr)
    : identifier(std::move(identifier)), type(std::move(type)), expr(std::move(expr)) {}


Function::Function(Token name, Token returnType, std::vector<std::unique_ptr<Parameter>> parameters, std::unique_ptr<Scope> body)
    : name(std::move(name)), returnType(std::move(returnType)), parameters(std::move(parameters)), body(std::move(body)) {}


Return::Return(std::unique_ptr<Expression> expr)
    : expr(std::move(expr)) {}
