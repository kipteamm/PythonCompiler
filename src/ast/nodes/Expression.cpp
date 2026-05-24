#include "Expression.h"

#include <stdexcept>


Binary::Binary(std::unique_ptr<Expression> lhs, Token operation, std::unique_ptr<Expression> rhs)
    : lhs(std::move(lhs)), rhs(std::move(rhs)), operation(std::move(operation)) {}

Unary::Unary(Token operation, std::unique_ptr<Expression> expr)
    : expr(std::move(expr)), operation(std::move(operation)) {}


Char::Char(const char value) : value(value) {}

Int::Int(const int value) : value(value) {}

Float::Float(const float value) : value(value) {}
