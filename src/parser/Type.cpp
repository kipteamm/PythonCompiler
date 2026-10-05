#include "Parser.h"


std::unique_ptr<Type> Parser::type() {
    std::vector<std::unique_ptr<Type>> types;
    types.push_back(singleType());

    // Consume singleTypes for as long as this is a valid union
    while (match(PIPE)) {
        types.push_back(singleType());
    }

    // If it wasn't an union return initial type only
    if (types.size() == 1)
        return std::move(types[0]);

    return std::make_unique<UnionType>(std::move(types));
}


std::unique_ptr<Type> Parser::singleType() {
    Token baseToken = peek();
    std::unique_ptr<Type> baseType;

    switch (baseToken.type) {
        case NONE:
            baseType = std::make_unique<PrimitiveType>(baseToken);
            advance(); // consume the type
            break;

        case IDENTIFIER:
            baseType = std::make_unique<UnresolvedType>(baseToken);
            advance(); // consume the type
            break;

        default:
            throw std::runtime_error("unexpected type '" + baseToken.lexeme + "'");
    }

    // Check whether this is a Generic type, if not return early
    if (!match(LEFT_BRACKET))
        return baseType;

    // Generic without arguments
    if (match(RIGHT_BRACKET))
        return std::make_unique<GenericType>(baseToken);

    std::vector<std::unique_ptr<Type>> arguments;
    arguments.push_back(type());

    while (match(COMMA)) {
        arguments.push_back(type());
    }
    consume(RIGHT_BRACKET, "expected ']' after generic type arguments");

    return std::make_unique<GenericType>(baseToken, std::move(arguments));
}
