#include "Lexer.h"

#include <iostream>
#include <utility>


inline bool isDigit(const char c) {
    return '0' <= c && c <= '9';
}

inline bool isAlpha(const char c) {
    return  ('a' <= c && c <= 'z') ||
            ('A' <= c && c <= 'Z') ||
            (c == '_');
}

inline bool isAlphaNumeric(const char c) {
    return isDigit(c) || isAlpha(c);
}


Lexer::Lexer(std::string source) : source(std::move(source)) {
    size_t pos = 0;

    // Some pre-lexing string tidying.
    //      1. \r\n linebreaks become \n as per the Python spec.
    while ((pos = this->source.find("\r\n", pos)) != std::string::npos) {
        this->source.replace(pos, 2, "\n");
    }
}


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

char Lexer::consume(const char expected, const std::string& error) {
    if (peek() == expected) return advance();
    throw std::runtime_error(error);
}


void Lexer::countIndents() {
    int indent = 0;
    while (peek() == ' ' || peek() == '\t') {
        indent += advance() == '\t'? 4: 1;
    }

    if (peek() == '\n' || peek() == '#' || atEnd()) return;

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


int Lexer::isPrefix(const char c) const {
    // f-string (string interpolation)
    const bool isF = c == 'f' || c == 'F';
    // r-string (raw strings: backslash is literal)
    const bool isR = c == 'r' || c == 'R';
    // b-string (byte strinsg)
    const bool isB = c == 'b' || c == 'B';

    if (!(isF || isR || isB)) return 0;
    // Next character starts a string so this is NOT COMBINED prefixed string
    if (peek() == '"' || peek() == '\'') return 1;

    // Maybe this is a combined prefixed string
    if (!(
        peek() == 'f' || peek() == 'F' ||
        peek() == 'r' || peek() == 'R' ||
        peek() == 'b' || peek() == 'B'
        )) return false;

    if (peek() == '"' || peek() == '\'') return 2;

    // Not a prefix
    return 0;
}


void Lexer::prefixedString(const char prefix) {
    switch (prefix) {
        case 'f':
        case 'F': {
            fStringType = stringType(fStringTerminator);

            addToken(F_STRING_START);
            start += 1;
            addStringOrChar(fStringType, fStringTerminator, true);
            start = current;

            return;
        }

        default:
            throw std::runtime_error("string prefix " + std::string(prefix, 1) + " not yet supported");
    }
}


TOKENTYPE Lexer::stringType(const char terminator) {
    TOKENTYPE type = terminator == '`'? CHARACTER: STRING;

    // Check for long strings and require certain syntax
    if (type != CHARACTER && peek() == terminator) {
        advance(); // Consume first extra terminator
        consume(terminator, "invalid syntax");
        type = LONG_STRING;
    }

    return type;
}


void Lexer::addStringOrChar(const TOKENTYPE type, const char terminator, const bool fString) {
    const int startCurrent = current;

    while (peek() != terminator && !atEnd()) {
        // Special condition for terminator `, which is used for characters
        // (which only consist of one unicode character...)
        if (terminator == '`' && startCurrent + 1 == current) break;
        // In the case of an f-string, consume everything up until first '{'
        // which starts an expression
        if (fString && peek() == '{') break;
        advance();
    }

    const bool isEnd = !fString || peek() == terminator || atEnd();

    if (isEnd)
        consume(terminator, "unterminated string literal");
    else consume('{', "lol");

    std::string value;
    if (type == LONG_STRING) {
        if (isEnd) {
            consume(terminator, "unterminated string literal");
            consume(terminator, "unterminated string literal");
        }

        value = source.substr(start + 3, current - start - 6);
    } else {
        value = source.substr(start + 1, current - start - 2);
    }

    if (!fString) {
        addToken(type, value);
        return;
    }

    if (isEnd) {
        if (!fStringStack.empty())
            throw std::runtime_error("invalid f-string");

        addToken(F_STRING_TEXT, value);
        addToken(F_STRING_END, "\"");
        return;
    }

    fStringStack.push_back(1);
    addToken(F_STRING_TEXT, value);
    addToken(LEFT_BRACE, "{");
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
        case '[': addToken(LEFT_BRACKET); break;
        case ']': addToken(RIGHT_BRACKET); break;
        case ':': addToken(COLON); break;
        case ',': addToken(COMMA); break;
        case '~': addToken(INVERSE); break;
        case '|': addToken(PIPE); break;

        // Curly braces
        case '{': {
            addToken(LEFT_BRACE);

            // If we are not parsing f-string, this is just a token
            if (fStringStack.empty()) break;
            fStringStack.back()++; break;
        }
        case '}': {
            addToken(RIGHT_BRACE);

            // If we are not parsing f-string, this is just a token
            if (fStringStack.empty()) break;
            fStringStack.back()--;

            if (fStringStack.back() > 0) break;
            fStringStack.pop_back();
            addStringOrChar(fStringType, fStringTerminator, true); break;
        }

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
            addToken(match('>')? ARROW: match('=')? MINUS_EQUAL: MINUS); break;
        case '+':
            addToken(match('=')? PLUS_EQUAL: PLUS); break;
        case '*':
            addToken(match('*')? match('=')? EXPONENT_EQUAL: EXPONENT: match('=')? STAR_EQUAL: STAR); break;
        case '/':
            addToken(match('/')? match('/')? FLOOR_EQUAL: FLOOR: match('=')? SLASH_EQUAL: SLASH); break;
        case '%':
            addToken(match('=')? MODULO_EQUAL: MODULO); break;

        // String literals
        case '"':
        case '\'':
        case '`':
            addStringOrChar(stringType(c), c, false); break;

        // Comments
        case '#': {
            while (peek() != '\n' && !atEnd()) advance();
            //addToken(COMMENT); break;
            break;
        }

        default:
            if (isDigit(c))
                addNumber(INTEGER);
            // isPrefix returns 0, 1, or 2 depending on how many prefixes are
            // used.
            else if (const int prefix = isPrefix(c); prefix) {
                if (prefix == 1) {
                    fStringTerminator = advance();
                    prefixedString(c);
                    break;
                }

                throw std::runtime_error("combined prefixes don't work yet and will never because holy fucking shit (give me some time to cope)");
            }
            else if (isAlpha(c))
                addIdentifier();
            else
                std::cerr << line << " unexpected character. '" << c << "'" << std::endl;
    }
}
