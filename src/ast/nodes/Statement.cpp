#include "Statement.h"

#include <utility>


Comment::Comment(std::string comment)
    : comment(std::move(comment)) {}


Assignment::Assignment(Token identifier, std::unique_ptr<Type> type, std::unique_ptr<Expression> expr)
    : identifier(std::move(identifier)), type(std::move(type)), expr(std::move(expr)) {}


Discard::Discard(std::unique_ptr<Expression> expr)
    : expr(std::move(expr)) {}


ForEach::ForEach(Token identifier, std::unique_ptr<Expression> iterable, std::unique_ptr<Scope> bodyScope, std::unique_ptr<Scope> elseScope)
    : identifier(std::move(identifier)), iterable(std::move(iterable)), bodyScope(std::move(bodyScope)), elseScope(std::move(elseScope)) {}


Function::Function(Token name, std::vector<std::unique_ptr<TypeParameter>> typeParameters, std::unique_ptr<Type> returnType, std::vector<std::unique_ptr<Parameter>> parameters, std::unique_ptr<Scope> body)
    : name(std::move(name)), typeParameters(std::move(typeParameters)), returnType(std::move(returnType)), parameters(std::move(parameters)), body(std::move(body)) {}


If::If(std::unique_ptr<Expression> condition, std::unique_ptr<Scope> thenScope, std::unique_ptr<Scope> elseScope)
    : condition(std::move(condition)), thenScope(std::move(thenScope)), elseScope(std::move(elseScope)) {}


Return::Return(std::unique_ptr<Expression> expr)
    : expr(std::move(expr)) {}


While::While(std::unique_ptr<Expression> condition, std::unique_ptr<Scope> bodyScope, std::unique_ptr<Scope> elseScope)
    : condition(std::move(condition)), bodyScope(std::move(bodyScope)), elseScope(std::move(elseScope)) {}
