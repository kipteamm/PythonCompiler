#include "Statement.h"

#include <utility>


Comment::Comment(std::string comment)
    : comment(std::move(comment)) {}


Assignment::Assignment(Token identifier, Token type, std::unique_ptr<Expression> expr)
    : identifier(std::move(identifier)), type(std::move(type)), expr(std::move(expr)){}
