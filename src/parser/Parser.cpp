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
        // Both declarations and assignments are rather ambigious in Python, so
        // we assume everything is an assignment. This is later properly
        // handled to assure that the first assignment is also a declaration.
        case IDENTIFIER:  return assignment();
        case DEF:         return function();
        case COMMENT:     return comment();
        case RETURN:      return return_();
        default:
            throw std::runtime_error("Failed to parse statement, got " + tokenTypeToString(peek().type) + " at " + std::to_string(current));
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

        // If the next token is a comma we build a new parameter, if next token
        // is a RIGHT_PAREN we found all paremeters.
        if (match(COMMA) || peek().type == RIGHT_PAREN) continue;

        // expected ',' or ')' in parameter list
        break;
    }

    consume(ARROW, "expected '->' return type specifier");
    const Token& returnType = consume(isType, "expected valid return type");

    consume(COLON, "expected ':' after function signature");

    auto scope = this->scope();

    return std::make_unique<Function>(identifier, returnType, std::move(parameters), std::move(scope));
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
    advance(); // RETUNR

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

        expr = std::make_unique<Unary>(std::move(operation), std::move(lhs));
    } else if (rhs == nullptr) {
        throw std::runtime_error("invalid syntax");
    } else {
        if (operation.type == INVERSE)
            throw std::runtime_error("inverse requires unary expression");

        expr = std::make_unique<Binary>(std::move(lhs), std::move(operation), std::move(rhs));
    }

    return expression(std::move(expr));
}


std::unique_ptr<Expression> Parser::primary() {
    switch (peek().type) {
        case IDENTIFIER: return std::make_unique<Identifier>(advance().lexeme); break;
        case CHARACTER:  return std::make_unique<Char>(advance().lexeme[0]);
        case INTEGER:    return std::make_unique<Int>(std::stoi(advance().lexeme));
        case FRACTION:   return std::make_unique<Float>(std::stof(advance().lexeme));
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



std::unique_ptr<Literal> Parser::literal(const Token &token) const {
    switch (token.type) {
        case CHARACTER:  return std::make_unique<Char>(token.lexeme[0]);
        case INTEGER:    return std::make_unique<Int>(std::stoi(token.lexeme));
        case FRACTION:   return std::make_unique<Float>(std::stof(token.lexeme));
        default:
            throw std::runtime_error("Failed to parse literal, got " + tokenTypeToString(token.type) + " at " + std::to_string(current));
    }
}
