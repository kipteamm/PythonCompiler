#include "Lexer.h"

#include <iostream>
#include <utility>


Lexer::Lexer(std::string source) : source(std::move(source)) {}


std::vector<Token> Lexer::scan() {
    while (!atEnd()) {
        start = current;
        scanSource();
    }

    tokens.emplace_back(END, "EOF");
    return tokens;
}


bool Lexer::atEnd() const {
    return current >= source.length();
}


bool Lexer::match(const char expected) {
    if (atEnd()) return false;
    if (source.at(current) != expected) return false;

    current++;
    return true;
}

char Lexer::peek() const {
    if (atEnd()) return '\0';
    return source.at(current);
}


char Lexer::advance() {
    return source.at(current++);
}


void Lexer::addStringOrChar(const char terminator) {
    const int startCurrent = current;

    while (peek() != terminator && !atEnd()) {
        // Special condition for terminator `, which is used for characters
        // (which only consist of one unicode character...)
        if (terminator == '`' && startCurrent + 1 == current) break;
        advance();
    }

    // TODO: check whether characters are correclty handled and whether or not I should do a specific check here
    advance();

    // Remove one to only store actual content
    const std::string value = source.substr(start + 1, current - start - 2);
    addToken(terminator == '`'? CHAR: STRING, value);
}


void Lexer::addToken(TokenType type, const std::string &lexeme) {
    tokens.emplace_back(type, lexeme);
}

void Lexer::addToken(TokenType type) {
    const std::string lexeme = source.substr(start, current - start);
    tokens.emplace_back(type, lexeme);
}


void Lexer::scanSource() {
    const char c = advance();

    switch (c) {
        // Ignores
        case ' ':
        case '\n': // <-- TODO
            break;

        // One character lexemes
        case '(': addToken(LEFT_PAREN); break;
        case ')': addToken(RIGHT_PAREN); break;
        case '{': addToken(LEFT_BRACE); break;
        case '}': addToken(RIGHT_BRACE); break;
        case '[': addToken(LEFT_BRACKET); break;
        case ']': addToken(RIGHT_BRACKET); break;
        case ':': addToken(COLON); break;
        case ',': addToken(COMMA); break;
        case '.': addToken(DOT); break;
        case '+': addToken(PLUS); break;
        case '*': addToken(STAR); break;
        case '/': addToken(SLASH); break;

        // Double character lexemes
        case '!':
            addToken(match('=')? BANG_EQUAL: BANG); break;
        case '=':
            addToken(match('=')? EQUAL_EQUAL: EQUAL); break;
        case '<':
            addToken(match('=')? LESS_EQUAL: LESS); break;
        case '>':
            addToken(match('=')? GREATER_EQUAL: GREATER); break;
        case '-':
            addToken(match('>')? ARROW: MINUS); break;

        // String literals
        case '"':
        case '\'':
        case '`':
            addStringOrChar(c); break;

        // Comments
        case '#':
            while (peek() != '\n' && !atEnd()) advance();
            addToken(COMMENT); break;

        // Literals, Identifiers & Keywords
            // TODO

        default:
            std::cerr << line << " unexpected character. '" << c << "'" << std::endl;
    }

    start = current;
}
