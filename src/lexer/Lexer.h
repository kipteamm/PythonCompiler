#ifndef PYTHONCOMPILER_LEXER_H
#define PYTHONCOMPILER_LEXER_H

#include "../common/Token.h"

#include <string>
#include <vector>


class Lexer {
public:
    explicit Lexer(std::string  source);

    [[nodiscard]] std::vector<Token> scan();

private:
    [[nodiscard]] bool atEnd() const;
    [[nodiscard]] bool match(char expected);
    [[nodiscard]] char peek(int index) const;
    [[nodiscard]] char peek() const;

    char consume(char expected, const std::string& error);
    char advance();

    [[nodiscard]] bool isDigit(char c) const;
    [[nodiscard]] bool isAlpha(char c) const;
    [[nodiscard]] bool isAlphaNumeric(char c) const;

    void countIndents();

    void addStringOrChar(char terminator);
    void addNumber(TOKENTYPE type);
    void addIdentifier();

    void addToken(TOKENTYPE type, const std::string& lexeme);
    void addToken(TOKENTYPE type);

    void scanSource();

    std::string source;
    std::vector<Token> tokens;
    std::vector<int> indentStack = {0};

    int current = 0;
    int line = 0;
    int start = 0;
};


#endif //PYTHONCOMPILER_LEXER_H
