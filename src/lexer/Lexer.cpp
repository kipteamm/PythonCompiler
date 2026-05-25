#include "Lexer.h"

#include <iostream>
#include <utility>


Lexer::Lexer(std::string source) : source(std::move(source)) {}


std::vector<Token> Lexer::scan() {
    while (!atEnd()) {
        start = current;
        scanSource();
    }

    // Dedent back to start level
    while (indentStack.size() > 1) {
        addToken(DEDENT);
        indentStack.pop_back();
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


void Lexer::countIndents() {
    int indent = 0;

    while (peek() == ' ' || peek() == '\t') {
        indent += advance() == '\t'? 4: 1;
    }

    const int top = indentStack.back();

    if (indent == top) return;
    if (indent > top) {
        indentStack.push_back(indent);
        addToken(INDENT);
        return;
    }

    // indent < top
    while (indentStack.size() > 1 && indent != indentStack[indentStack.size() - 1]) {
        indentStack.pop_back();
        addToken(DEDENT);
    }

    if (indentStack.back() == indent) return;

    std::cerr << "IndentationError: unindent does not match any outer indentation level\n";
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

void Lexer::addNumber(TOKENTYPE type) {
    while (isDigit(peek())) advance();

    // Currently an int and next up we find a dot? Consume . and keep looking
    // for digits.
    if (type == INTEGER && peek() == '.' && isDigit(peek(current + 1))) {
        type = FRACTION;
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
    const TOKENTYPE type = (it != KEYWORDS.end()) ? it->second : IDENTIFIER;

    addToken(type, value);
}


void Lexer::addToken(TOKENTYPE type, const std::string &lexeme) {
    tokens.emplace_back(type, lexeme);
}

void Lexer::addToken(TOKENTYPE type) {
    const std::string lexeme = source.substr(start, current - start);
    tokens.emplace_back(type, lexeme);
}


void Lexer::scanSource() {
    const char c = advance();

    switch (c) {
        case '\n': countIndents(); break;

        // Indentation is already handled, skip anything left
        case ' ':
        case '\t': break;

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
        case '~': addToken(INVERSE); break;

        // Dot OR Fraction floating point
        case '.':
            if (isDigit(peek())) addNumber(FRACTION);
            else addToken(DOT);
            break;

        // Double character lexemes
        case '!':
            addToken(match('=')? BANG_EQUAL: BANG); break;
        case '=':
            addToken(match('=')? EQUAL_EQUAL: EQUAL); break;
        case '<':
            addToken(match('=')? LESS_EQUAL: match('<')? LEFT_SHIFT: LESS); break;
        case '>':
            addToken(match('=')? GREATER_EQUAL: match('>')? RIGHT_SHIFT: GREATER); break;
        case '-':
            addToken(match('>')? ARROW: MINUS); break;
        case '*':
            addToken(match('*')? EXPONENT: STAR); break;
        case '/':
            addToken(match('/')? FLOOR: SLASH); break;

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
            if (isDigit(c)) addNumber(INTEGER);
            else if (isAlpha(c)) addIdentifier();
            else std::cerr << line << " unexpected character. '" << c << "'" << std::endl;
    }
}
