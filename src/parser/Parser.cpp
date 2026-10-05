#include "Parser.h"

#include <algorithm>
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

bool Parser::match(const Assertion isToken) {
    if (current >= tokens.size()) throw std::runtime_error("expected token not found");
    if (!isToken(peek().type)) return false;

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
