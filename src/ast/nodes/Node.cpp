#include "Node.h"


Parameter::Parameter(Token type, Token identifier, std::unique_ptr<Expression> initValue)
    : type(std::move(type)), identifier(std::move(identifier)), initValue(std::move(initValue)) {}
