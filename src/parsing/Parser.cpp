#include "Parser.h"


Parser::Parser(const std::vector<Token> &tokens) : tokens(tokens) {}


Token Parser::peek() const {
    return tokens.at(current);
}

bool Parser::match(const TOKENTYPE token) {
    if (peek().type != token) return false;

    advance();
    return true;
}


Token Parser::advanceAssert(const Assertion assertion) {
    if (assertion(peek().type)) return advance();

    throw std::runtime_error("Unexpected token type");
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


std::unique_ptr<Statement> Parser::statement() {
    switch (peek().type) {
        // Both declarations and assignments are rather ambigious in Python, so
        // we assume everything is an assignment. This is later properly
        // handled to assure that the first assignment is also a declaration.
        case IDENTIFIER:  return assignment();
        case DEF:         return function();
        case COMMENT:     return comment();
        default:
            throw std::runtime_error("Failed to parse statement, got " + tokenTypeToString(peek().type) + " at " + std::to_string(current));
    }
}


std::unique_ptr<Declaration> Parser::assignment() {
    const Token id = advance();
    const Token type = match(COLON)? advanceAssert(isType): Token(UNKNOWN, "UNKNOWN");

    std::unique_ptr<Expression> expr = nullptr;
    if (match(EQUAL)) expr = expression();

    return std::make_unique<Declaration>(id, type, std::move(expr));
}


std::unique_ptr<Comment> Parser::comment() {
    const Token& token = advance();
    return std::make_unique<Comment>(token.lexeme);
}


std::unique_ptr<Function> Parser::function() {
    return std::make_unique<Function>();
}


std::unique_ptr<Expression> Parser::expression() {
    const Token& token = advance();
    if (isLiteral(token.type)) return literal(token);

    auto expr = std::make_unique<Expression>();

    return expr;
}


std::unique_ptr<Literal> Parser::literal(const Token &token) const {
    switch (token.type) {
        case CHARACTER:  return std::make_unique<Char>(token.lexeme[0]);
        case INTEGER:    return std::make_unique<Int>(std::stoi(token.lexeme));
        default:
            throw std::runtime_error("Failed to parse literal, got " + tokenTypeToString(token.type) + " at " + std::to_string(current));
    }
}
