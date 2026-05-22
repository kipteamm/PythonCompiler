#ifndef PYTHONCOMPILER_TOKEN_H
#define PYTHONCOMPILER_TOKEN_H

#include <unordered_map>
#include <ostream>
#include <string>


enum TokenType {
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE, LEFT_BRACKET, RIGHT_BRACKET,

    COMMA, DOT, MINUS, PLUS, SLASH, STAR,

    COLON, ARROW, TAB,

    BANG_EQUAL, BANG, EQUAL_EQUAL, EQUAL, LESS_EQUAL, LESS, GREATER_EQUAL, GREATER,

    // Literals
    CHARACTER, STRING, INTEGER, FLOAT,
    IDENTIFIER,

    // Keywords:
    FALSE, NONE, TRUE, AND, AS, ASSERT, ASYNC, AWAIT, BREAK, CHAR, CLASS,
    CONTINUE, DEF, DEL, ELIF, ELSE, EXCEPT, FINALLY, FOR, FROM,
    GLOBAL, IF, IMPORT, IN, INT, IS, LAMBDA, NONLOCAL, NOT, OR, PASS,
    RAISE, RETURN, TRY, WHILE, WITH, YIELD,

    COMMENT,
    END // End Of File (EOF is reserved)
};


static std::unordered_map<std::string, TokenType> KEYWORDS = {
    {"False",    FALSE},
    {"None",     NONE},
    {"True",     TRUE},
    {"and",      AND},
    {"as",       AS},
    {"assert",   ASSERT},
    {"async",    ASYNC},
    {"await",    AWAIT},
    {"break",    BREAK},
    {"char",     CHAR},
    {"class",    CLASS},
    {"continue", CONTINUE},
    {"def",      DEF},
    {"del",      DEL},
    {"elif",     ELIF},
    {"else",     ELSE},
    {"except",   EXCEPT},
    {"finally",  FINALLY},
    {"for",      FOR},
    {"from",     FROM},
    {"global",   GLOBAL},
    {"if",       IF},
    {"import",   IMPORT},
    {"in",       IN},
    {"int",      INT},
    {"is",       IS},
    {"lambda",   LAMBDA},
    {"nonlocal", NONLOCAL},
    {"not",      NOT},
    {"or",       OR},
    {"pass",     PASS},
    {"raise",    RAISE},
    {"return",   RETURN},
    {"try",      TRY},
    {"while",    WHILE},
    {"with",     WITH},
    {"yield",    YIELD}
};


inline std::string tokenTypeToString(const TokenType type) {
    switch (type) {
        case LEFT_PAREN:     return "LEFT_PAREN";
        case RIGHT_PAREN:    return "RIGHT_PAREN";
        case LEFT_BRACE:     return "LEFT_BRACE";
        case RIGHT_BRACE:    return "RIGHT_BRACE";
        case LEFT_BRACKET:   return "LEFT_BRACKET";
        case RIGHT_BRACKET:  return "RIGHT_BRACKET";

        case COMMA:          return "COMMA";
        case DOT:            return "DOT";
        case MINUS:          return "MINUS";
        case PLUS:           return "PLUS";
        case SLASH:          return "SLASH";
        case STAR:           return "STAR";

        case COLON:          return "COLON";
        case ARROW:          return "ARROW";
        case TAB:            return "TAB";

        case BANG_EQUAL:     return "BANG_EQUAL";
        case BANG:           return "BANG";
        case EQUAL_EQUAL:    return "EQUAL_EQUAL";
        case EQUAL:          return "EQUAL";
        case LESS_EQUAL:     return "LESS_EQUAL";
        case LESS:           return "LESS";
        case GREATER_EQUAL:  return "GREATER_EQUAL";
        case GREATER:        return "GREATER";

        case CHARACTER:      return "CHARACTER";
        case STRING:         return "STRING";
        case INTEGER:        return "INTEGER";
        case FLOAT:          return "FLOAT";
        case IDENTIFIER:     return "IDENTIFIER";

        // Keywords
        case FALSE:          return "FALSE";
        case NONE:           return "NONE";
        case TRUE:           return "TRUE";
        case AND:            return "AND";
        case AS:             return "AS";
        case ASSERT:         return "ASSERT";
        case ASYNC:          return "ASYNC";
        case AWAIT:          return "AWAIT";
        case BREAK:          return "BREAK";
        case CHAR:           return "CHAR";
        case CLASS:          return "CLASS";
        case CONTINUE:       return "CONTINUE";
        case DEF:            return "DEF";
        case DEL:            return "DEL";
        case ELIF:           return "ELIF";
        case ELSE:           return "ELSE";
        case EXCEPT:         return "EXCEPT";
        case FINALLY:        return "FINALLY";
        case FOR:            return "FOR";
        case FROM:           return "FROM";
        case GLOBAL:         return "GLOBAL";
        case IF:             return "IF";
        case IMPORT:         return "IMPORT";
        case IN:             return "IN";
        case INT:            return "INT";
        case IS:             return "IS";
        case LAMBDA:         return "LAMBDA";
        case NONLOCAL:       return "NONLOCAL";
        case NOT:            return "NOT";
        case OR:             return "OR";
        case PASS:           return "PASS";
        case RAISE:          return "RAISE";
        case RETURN:         return "RETURN";
        case TRY:            return "TRY";
        case WHILE:          return "WHILE";
        case WITH:           return "WITH";
        case YIELD:          return "YIELD";

        case COMMENT:        return "COMMENT";
        case END:            return "EOF";

        default:             return "UNKNOWN_TOKEN";
    }
}


[[nodiscard]] inline std::string escapeLexeme(const std::string& lexeme) {
    std::string escaped;
    escaped.reserve(lexeme.length());

    for (const char c : lexeme) {
        switch (c) {
            case '\t': escaped += "\\t"; break;
            case '\n': escaped += "\\n"; break;
            case '\r': escaped += "\\r"; break;
            default:   escaped += c;     break;
        }
    }

    return escaped;
}


struct Token {
    TokenType type;
    std::string lexeme;

    explicit Token(const TokenType type, std::string lexeme)
        : type(type), lexeme(std::move(lexeme)) {}

    [[nodiscard]] friend std::ostream& operator<<(std::ostream& stream, const Token& token) {
        stream << tokenTypeToString(token.type) << " (" << escapeLexeme(token.lexeme) << ")";
        return stream;
    }
};


#endif //PYTHONCOMPILER_TOKEN_H
