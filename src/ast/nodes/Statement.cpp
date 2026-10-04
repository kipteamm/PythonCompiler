#include "Statement.h"

#include <utility>


Comment::Comment(std::string comment)
    : comment(std::move(comment)) {}


Assignment::Assignment(Token identifier, std::unique_ptr<Type> type, std::unique_ptr<Expression> expr)
    : identifier(std::move(identifier)), type(std::move(type)), expr(std::move(expr)) {}


Case::Case(std::unique_ptr<Pattern> pattern, std::unique_ptr<Expression> guard, std::unique_ptr<Scope> body)
    : pattern(std::move(pattern)), guard(std::move(guard)), body(std::move(body)) {}



Discard::Discard(std::unique_ptr<Expression> expr)
    : expr(std::move(expr)) {}


ForEach::ForEach(Token identifier, std::unique_ptr<Expression> iterable, std::unique_ptr<Scope> bodyScope, std::unique_ptr<Scope> elseScope)
    : identifier(std::move(identifier)), iterable(std::move(iterable)), bodyScope(std::move(bodyScope)), elseScope(std::move(elseScope)) {}


Function::Function(Token name, std::vector<std::unique_ptr<TypeParameter>> typeParameters, std::unique_ptr<Type> returnType, std::vector<std::unique_ptr<Parameter>> parameters, std::unique_ptr<Scope> body)
    : name(std::move(name)), typeParameters(std::move(typeParameters)), returnType(std::move(returnType)), parameters(std::move(parameters)), body(std::move(body)) {}


If::If(std::unique_ptr<Expression> condition, std::unique_ptr<Scope> thenScope, std::unique_ptr<Scope> elseScope)
    : condition(std::move(condition)), thenScope(std::move(thenScope)), elseScope(std::move(elseScope)) {}


Match::Match(std::unique_ptr<Expression> expression, std::vector<std::unique_ptr<Case>> cases)
    : expression(std::move(expression)), cases(std::move(cases)) {}


Return::Return(std::unique_ptr<Expression> expr)
    : expr(std::move(expr)) {}


While::While(std::unique_ptr<Expression> condition, std::unique_ptr<Scope> bodyScope, std::unique_ptr<Scope> elseScope)
    : condition(std::move(condition)), bodyScope(std::move(bodyScope)), elseScope(std::move(elseScope)) {}
