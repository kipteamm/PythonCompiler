#include "Node.h"
#include "Type.h"


Parameter::Parameter(Token identifier, std::unique_ptr<Type> type, std::unique_ptr<Expression> initValue)
    : identifier(std::move(identifier)), type(std::move(type)), initValue(std::move(initValue)) {}
