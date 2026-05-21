#ifndef PYTHONCOMPILER_TOKEN_H
#define PYTHONCOMPILER_TOKEN_H

#include <ostream>
#include <string>


enum TokenType {
    // Parentheses, Braces, and Brackets
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE, LEFT_BRACKET, RIGHT_BRACKET,

    // Single character operators/punctuation
    COMMA, DOT, MINUS, PLUS, SLASH, STAR,

    // Typing and structures
    COLON, ARROW,

    // Literals
    BANG_EQUAL, BANG, EQUAL_EQUAL, EQUAL, LESS_EQUAL, LESS, GREATER_EQUAL, GREATER,

    // Comments
    CHAR, STRING,

    // End of File
    COMMENT,

    // End Of File (EOF is reserved)
    END
};


inline std::string tokenTypeToString(const TokenType type) {
    switch (type) {
        // Parentheses, Braces, and Brackets
        case LEFT_PAREN:     return "LEFT_PAREN";
        case RIGHT_PAREN:    return "RIGHT_PAREN";
        case LEFT_BRACE:     return "LEFT_BRACE";
        case RIGHT_BRACE:    return "RIGHT_BRACE";
        case LEFT_BRACKET:   return "LEFT_BRACKET";
        case RIGHT_BRACKET:  return "RIGHT_BRACKET";

        // Single character operators/punctuation
        case COMMA:          return "COMMA";
        case DOT:            return "DOT";
        case MINUS:          return "MINUS";
        case PLUS:           return "PLUS";
        case SLASH:          return "SLASH";
        case STAR:           return "STAR";

        // Typing and structures
        case COLON:          return "COLON";
        case ARROW:          return "ARROW";

        // Comparison and Assignment operators
        case BANG_EQUAL:     return "BANG_EQUAL";
        case BANG:           return "BANG";
        case EQUAL_EQUAL:    return "EQUAL_EQUAL";
        case EQUAL:          return "EQUAL";
        case LESS_EQUAL:     return "LESS_EQUAL";
        case LESS:           return "LESS";
        case GREATER_EQUAL:  return "GREATER_EQUAL";
        case GREATER:        return "GREATER";

        // Literals
        case CHAR:           return "CHAR";
        case STRING:         return "STRING";

        case COMMENT:        return "COMMENT";
        case END:            return "EOF";

        default:             return "UNKNOWN_TOKEN";
    }
}

struct Token {
    TokenType type;
    std::string lexeme;

    explicit Token(const TokenType type, std::string lexeme)
        : type(type), lexeme(std::move(lexeme)) {}

    [[nodiscard]] friend std::ostream& operator<<(std::ostream& stream, const Token& token) {
        stream << tokenTypeToString(token.type) << " (" << token.lexeme << ")";
        return stream;
    }
};


#endif //PYTHONCOMPILER_TOKEN_H
