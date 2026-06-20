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

    void countIndents();

    // Prefixed strings & f-strings
    [[nodiscard]] int isPrefix(char c) const;
    void prefixedString(char prefix);

    // Literals
    [[ nodiscard ]] TOKENTYPE stringType(char terminator);
    void addStringOrChar(TOKENTYPE type, char terminator, bool fString);
    void addNumber(TOKENTYPE type);
    void addIdentifier();

    void addToken(TOKENTYPE type, const std::string& lexeme);
    void addToken(TOKENTYPE type);

    void scanSource();

    std::string source;
    std::vector<Token> tokens;
    std::vector<int> indentStack = {0};

    // fstring
    std::vector<int> fStringStack = {};
    TOKENTYPE fStringType = UNKNOWN;
    char fStringTerminator = '\0';

    int current = 0;
    int line = 0;
    int start = 0;
};


#endif //PYTHONCOMPILER_LEXER_H
