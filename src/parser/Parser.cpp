#include "Parser.h"

#include <iostream>


Parser::Parser(const std::vector<Token> &tokens) : tokens(tokens) {}


Token Parser::peek() const {
    return tokens.at(current);
}

bool Parser::match(const TOKENTYPE token) {
    if (current >= tokens.size()) throw std::runtime_error("expected token " + tokenTypeToString(token) + " not found");
    if (peek().type != token) return false;

    advance();
    return true;
}


Token Parser::consume(const Assertion assertion, const std::string& error) {
    if (assertion(peek().type)) return advance();

    throw std::runtime_error(error);
}

Token Parser::consume(const TOKENTYPE type, const std::string& error) {
    if (peek().type == type) return advance();

    throw std::runtime_error(error);
}



Token Parser::advance() {
    return tokens.at(current++);
}


std::unique_ptr<Scope> Parser::start() {
    auto scope = std::make_unique<Scope>();

    while (!match(END)) {
        scope->addStatement(statement());
    }

    return scope;
}


std::unique_ptr<Scope> Parser::scope() {
    consume(INDENT, "wrong indentation level");

    auto scope = std::make_unique<Scope>();

    while (!match(DEDENT)) {
        scope->addStatement(statement());
    }

    return scope;
}


std::unique_ptr<Statement> Parser::statement() {
    switch (peek().type) {
        case IDENTIFIER: {
            // This could be either a standalone expression, or a variable
            // assignment/declaration. Depends on what follows, eg;

            if (tokens.at(current + 1).type == EQUAL || tokens.at(current + 1).type == COLON) {
                // Both declarations and assignments are rather ambigious in Python, so
                // we assume everything is an assignment. This is later properly
                // handled to assure that the first assignment is also a declaration.
                return assignment();
            }

            // Global expressions are technically discarded expressions, tho
            // FunctionCalls do still have effect on the program
            auto expr = expression(std::move(primary()));
            return std::make_unique<Discard>(std::move(expr));
        }

        case DEF:         return function();
        case COMMENT:     return comment();
        case IF:          return if_();
        case RETURN:      return return_();

        default: {
            auto value = primary();
            if (value == nullptr)
                throw std::runtime_error("Failed to parse statement, got " + tokenTypeToString(peek().type) + " at " + std::to_string(current));

            return std::make_unique<Discard>(std::move(value));
        }
    }
}


std::unique_ptr<Assignment> Parser::assignment() {
    const Token identifier = consume(IDENTIFIER, "expected identifier");
    const Token type = match(COLON)
        ? consume(isType, "expected type")
        : Token(UNKNOWN, "UNKNOWN");

    std::unique_ptr<Expression> expr = nullptr;
    if (match(EQUAL)) expr = expression(std::move(primary()));

    return std::make_unique<Assignment>(identifier, type, std::move(expr));
}


std::unique_ptr<Comment> Parser::comment() {
    const Token& comment = consume(COMMENT, "expected comment");
    return std::make_unique<Comment>(comment.lexeme);
}


std::unique_ptr<Function> Parser::function() {
    advance(); // DEF keyword

    const Token& identifier = consume(IDENTIFIER, "expected function name");
    consume(LEFT_PAREN, "expected '(' after function name");

    std::vector<std::unique_ptr<Parameter>> parameters;

    while (!match(RIGHT_PAREN)) {
        parameters.push_back(parameter());

        if (peek().type == RIGHT_PAREN) continue;
        consume(COMMA, "Expected next argument");
    }

    consume(ARROW, "expected '->' return type specifier");
    const Token& returnType = consume(isType, "expected valid return type");

    consume(COLON, "expected ':' after function signature");

    auto scope = this->scope();

    return std::make_unique<Function>(identifier, returnType, std::move(parameters), std::move(scope));
}


std::unique_ptr<If> Parser::if_() {
    advance(); // IF keyword

    auto condition = expression(std::move(primary()));
    consume(COLON, "expected ':'");

    auto thenScope = scope();
    std::unique_ptr<Scope> elseScope = nullptr;

    if (match(ELSE)) {
        consume(COLON, "expected ':'");
        elseScope = scope();
    } else if (peek().type == ELIF) {
        elseScope = std::make_unique<Scope>();

        auto elif = if_();
        elseScope->addStatement(std::move(elif));
    }

    return std::make_unique<If>(std::move(condition), std::move(thenScope), std::move(elseScope));
}


std::unique_ptr<Parameter> Parser::parameter() {
    const Token& identifier = consume(IDENTIFIER, "expected parameter name");
    consume(COLON, "expected ':' after paremeter name");
    const Token& type = consume(isType, "expected a type for parameter");

    std::unique_ptr<Expression> expr = nullptr;
    if (match(EQUAL)) expr = expression(std::move(primary()));

    return std::make_unique<Parameter>(type, identifier, std::move(expr));
}


std::unique_ptr<Return> Parser::return_() {
    advance(); // RETURN

    auto expr = expression(std::move(primary()));

    return std::make_unique<Return>(std::move(expr));
}


std::unique_ptr<Expression> Parser::expression(std::unique_ptr<Expression> lhs) {
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
        if (operation.type == INVERSE)
            throw std::runtime_error("inverse requires unary expression");

        expr = std::make_unique<Binary>(std::move(lhs), std::move(operation), std::move(rhs));
    }

    return expression(std::move(expr));
}


std::unique_ptr<FunctionCall> Parser::functionCall(const Token& token) {
    std::vector<std::unique_ptr<Expression>> arguments;

    while (!match(RIGHT_PAREN)) {
        arguments.push_back(expression(std::move(primary())));

        if (peek().type == RIGHT_PAREN) continue;
        consume(COMMA, "Expected next argument");
    }

    return std::make_unique<FunctionCall>(token, std::move(arguments));
}



std::unique_ptr<Expression> Parser::primary() {
    switch (peek().type) {
        case IDENTIFIER: {
            // Identifier literals can either be a
            //  - a function call: the token is followed by a '('
            //  - variable identifier: the token is not followed by anuthing of
            //    signficicance

            const auto identifier = advance();
            if (match(LEFT_PAREN))
                return functionCall(identifier);

            return std::make_unique<Identifier>(identifier.lexeme);
        }

        case FALSE:
        case TRUE:           return std::make_unique<Bool>(advance().type == TRUE);

        case CHARACTER:      return std::make_unique<Char>(advance().lexeme[0]);
        case FRACTION:       return std::make_unique<Float>(std::stof(advance().lexeme));
        case INTEGER:        return std::make_unique<Int>(std::stoi(advance().lexeme));

        case STRING:
        case LONG_STRING:    return std::make_unique<String>(std::move(advance().lexeme));

        case F_STRING_START: return fString();

        // '(' expression ')'
        case LEFT_PAREN: {
            advance(); // (
            auto expr = expression(std::move(primary()));
            consume(RIGHT_PAREN, "missing closing bracket");

            return expr;
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

