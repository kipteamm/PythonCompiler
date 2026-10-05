#include "Parser.h"


std::unique_ptr<Expression> Parser::expression() {
    return std::move(expression_(std::move(primary())));
}


std::unique_ptr<Expression> Parser::expression_(std::unique_ptr<Expression> lhs) {
    // Just primaries no operations
    if (!isOperation(peek().type) && lhs != nullptr) return lhs;

    Token operation = consume(isOperation, "expected operator");
    auto rhs = primary();

    std::unique_ptr<Expression> expr;
    if (lhs == nullptr) {
        if (!isUnaryOperation(operation.type))
            throw std::runtime_error("not a unary operation");

        expr = std::make_unique<Unary>(std::move(operation), std::move(rhs));
    } else if (rhs == nullptr) {
        throw std::runtime_error("invalid syntax");
    } else {
        if (isOnlyUnaryOperation(operation.type))
            throw std::runtime_error("unary operation expressed as binary operation");

        expr = std::make_unique<Binary>(std::move(lhs), std::move(operation), std::move(rhs));
    }

    return expression_(std::move(expr));
}


std::unique_ptr<FunctionCall> Parser::functionCall(const Token& token, std::vector<std::unique_ptr<Type>> typeArguments) {
    std::vector<std::unique_ptr<Expression>> arguments;

    while (!match(RIGHT_PAREN)) {
        arguments.push_back(expression());

        if (peek().type == RIGHT_PAREN) continue;
        consume(COMMA, "Expected next argument");
    }

    return std::make_unique<FunctionCall>(token, std::move(typeArguments), std::move(arguments));
}


std::unique_ptr<Literal> Parser::literal() {
    switch (peek().type) {
        // In the case of a literal, it may be preceeded by a minus or a plus,
        // as long as the operator is not followed by something that isn't a
        // literal
        case PLUS:
        case MINUS: {
            const TOKENTYPE next = tokens.at(current + 1).type;
            const bool negative = peek().type == MINUS;

            if (next == INTEGER) {
                advance();
                return std::make_unique<Int>(std::stoi(advance().lexeme) * (negative? -1: 1));
            }

            if (next == FRACTION) {
                advance();
                return std::make_unique<Float>(std::stof(advance().lexeme) * (negative? -1.0: 1.0));
            }

            return nullptr;
        }

        case FALSE:
        case TRUE:          return std::make_unique<Bool>(advance().type == TRUE);

        case CHARACTER:     return std::make_unique<Char>(advance().lexeme[0]);
        case FRACTION:      return std::make_unique<Float>(std::stof(advance().lexeme));
        case INTEGER:       return std::make_unique<Int>(std::stoi(advance().lexeme));

        case STRING:
        case LONG_STRING:   return std::make_unique<String>(std::move(advance().lexeme));

        case NONE: {
            advance(); // consume NONE token
            return std::make_unique<None>();
        }

        default: return nullptr;
    }
}


std::unique_ptr<Expression> Parser::primary() {
    auto literal = this->literal();
    if (literal != nullptr)
        return literal;

    switch (peek().type) {
        case IDENTIFIER: {
            // Identifier literals can either be a
            //  - a function call: the token is followed by a '('
            //  - a function call: the token is followed by a '[' -> explicit
            //    generic arguments
            //  - variable identifier: the token is not followed by anuthing of
            //    signficicance
            const auto identifier = advance();

            // Check for explicit generic type arguments
            std::vector<std::unique_ptr<Type>> typeArgs = {};

            if (match(LEFT_BRACKET)) {
                do {
                    typeArgs.push_back(type());
                } while (match(COMMA));

                consume(RIGHT_BRACKET, "expected ']' after type arguments");
            }

            if (match(LEFT_PAREN))
                return functionCall(identifier, std::move(typeArgs));

            return std::make_unique<Identifier>(identifier.lexeme);
        }

        case F_STRING_START: return fString();

        // '(' expression ')'
        case LEFT_PAREN: {
            advance(); // (
            auto expr = expression();
            consume(RIGHT_PAREN, "missing closing bracket");

            return expr;
        }

        // List
        case LEFT_BRACKET: {
            advance(); // [

            // Empty list
            if (match(RIGHT_BRACKET))
                return std::make_unique<List>();

            std::vector<std::unique_ptr<Expression>> values;
            values.push_back(expression());

            while (match(COMMA)) {
                values.push_back(expression());
            }

            consume(RIGHT_BRACKET, "list not closed");
            return std::make_unique<List>(std::move(values));
        }

        // Dictionary
        case LEFT_BRACE: {
            advance(); // {

            // Empty dict
            if (match(RIGHT_BRACE))
                return std::make_unique<Dictionary>();

            std::vector<std::unique_ptr<Expression>> keys;
            std::vector<std::unique_ptr<Expression>> values;

            keyValue(keys, values);

            while (match(COMMA)) {
                keyValue(keys, values);
            }

            consume(RIGHT_BRACE, "dictionary not closed");
            return std::make_unique<Dictionary>(std::move(keys), std::move(values));
        }

        default: return nullptr;
    }
}


std::unique_ptr<JoinedString> Parser::fString() {
    consume(F_STRING_START, "");
    std::vector<std::unique_ptr<Expression>> values;

    while (!match(F_STRING_END)) {
        std::unique_ptr<Expression> value;
        auto next = peek();

        if (next.type == F_STRING_TEXT) {
            values.push_back(std::make_unique<String>(next.lexeme));
        } else {
            consume(LEFT_BRACE, "");
            values.push_back(std::move(primary()));
            consume(RIGHT_BRACE, "");
        }

        advance();
    }

    return std::make_unique<JoinedString>(std::move(values));
}


void Parser::keyValue(std::vector<std::unique_ptr<Expression>>& keys, std::vector<std::unique_ptr<Expression>>& values) {
    keys.push_back(expression());
    consume(COLON, "':' expected after dictionary key");
    values.push_back(expression());
}
