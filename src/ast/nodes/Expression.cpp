#include "Expression.h"

#include <utility>


Binary::Binary(std::unique_ptr<Expression> lhs, Token operation, std::unique_ptr<Expression> rhs)
    : lhs(std::move(lhs)), rhs(std::move(rhs)), operation(std::move(operation)) {}

Unary::Unary(Token operation, std::unique_ptr<Expression> expr)
    : expr(std::move(expr)), operation(std::move(operation)) {}


Dictionary::Dictionary() = default;

Dictionary::Dictionary(std::vector<std::unique_ptr<Expression>> keys, std::vector<std::unique_ptr<Expression>> values)
    : keys(std::move(keys)), values(std::move(values)) {}


FunctionCall::FunctionCall(Token identifier, std::vector<std::unique_ptr<Expression>> arguments)
    : identifier(std::move(identifier)), arguments(std::move(arguments)) {}


Identifier::Identifier(std::string name) : name(std::move(name)) {};


List::List() = default;

List::List(std::vector<std::unique_ptr<Expression>> values)
    : values(std::move(values)) {}


Bool::Bool(const bool value) : value(value) {}

Char::Char(const char value) : value(value) {}

Float::Float(const float value) : value(value) {}

Int::Int(const int value) : value(value) {}

String::String(std::string  value) : value(std::move(value)) {}

JoinedString::JoinedString(std::vector<std::unique_ptr<Expression>> values)
    : values(std::move(values)) {}

