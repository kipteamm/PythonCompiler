#include "Type.h"


GenericType::GenericType(Token base)
    : base(std::move(base)) {}

GenericType::GenericType(Token base, std::vector<std::unique_ptr<Type>> arguments)
    : base(std::move(base)), arguments(std::move(arguments)) {}


PrimitiveType::PrimitiveType(Token type)
    : type(std::move(type)) {}


UnionType::UnionType(std::vector<std::unique_ptr<Type>> types)
    : types(std::move(types)) {}


UnresolvedType::UnresolvedType(Token identifier)
    : identifier(std::move(identifier)) {}
