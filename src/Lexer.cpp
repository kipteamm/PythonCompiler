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

char Lexer::peek(const int index) const {
    if (atEnd()) return '\0';
    return source.at(index);
}

char Lexer::peek() const {
    return peek(current);
}


char Lexer::advance() {
    return source.at(current++);
}


bool Lexer::isDigit(const char c) const {
    return '0' <= c && c <= '9';
}

bool Lexer::isAlpha(const char c) const {
    return  ('a' <= c && c <= 'z') ||
            ('A' <= c && c <= 'Z') ||
            (c == '_');
}

bool Lexer::isAlphaNumeric(char c) const {
    return isDigit(c) || isAlpha(c);
}


void Lexer::addStringOrChar(const char terminator) {
    const int startCurrent = current;

    while (peek() != terminator && !atEnd()) {
        // Special condition for terminator `, which is used for characters
        // (which only consist of one unicode character...)
        if (terminator == '`' && startCurrent + 1 == current) break;
        advance();
    }

    advance();

    // Remove one to only store actual content
    const std::string value = source.substr(start + 1, current - start - 2);
    addToken(terminator == '`'? CHARACTER: STRING, value);
}

void Lexer::addNumber() {
    while (isDigit(peek())) advance();

    TokenType type = INTEGER;

    // Fraction? If so consume . and keep looking for digits.
    if (peek() == '.' && isDigit(peek(current + 1))) {
        type = FLOAT;
        advance();

        while (isDigit(peek())) advance();
    }

    const std::string value = source.substr(start, current - start);
    addToken(type, value);
}

void Lexer::addIdentifier() {
    while (isAlphaNumeric(peek())) advance();

    const std::string value = source.substr(start, current - start);

    // Check whether found identifier is a reserved keyword. Change type to
    // IDENTIFIER otherwise
    const auto it = KEYWORDS.find(value);
    const TokenType type = (it != KEYWORDS.end()) ? it->second : IDENTIFIER;

    addToken(type, value);
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
        case '\n':
            break;

        // Whitespace
        case '\t': addToken(TAB); break;

        // One character lexemes
        case '(': addToken(LEFT_PAREN); break;
        case ')': addToken(RIGHT_PAREN); break;
        case '{': addToken(LEFT_BRACE); break;
        case '}': addToken(RIGHT_BRACE); break;
        case '[': addToken(LEFT_BRACKET); break;
        case ']': addToken(RIGHT_BRACKET); break;
        case ':': addToken(COLON); break;
        case ',': addToken(COMMA); break;
        case '+': addToken(PLUS); break;
        case '*': addToken(STAR); break;
        case '/': addToken(SLASH); break;

        // Dot OR Floating point
        case '.':
            if (isDigit(peek())) addNumber();
            else addToken(DOT);
            break;

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

        default:
            if (isDigit(c)) addNumber();
            else if (isAlpha(c)) addIdentifier();
            else std::cerr << line << " unexpected character. '" << c << "'" << std::endl;
    }

    start = current;
}
