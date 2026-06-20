#ifndef PYTHONCOMPILER_TOKEN_H
#define PYTHONCOMPILER_TOKEN_H

#include <unordered_map>
#include <ostream>
#include <string>


enum TOKENTYPE {
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE, LEFT_BRACKET, RIGHT_BRACKET,

    COLON, ARROW, INDENT, DEDENT, COMMA, DOT,

    // Unary
    INVERSE,
    // Both
    MINUS, PLUS, STAR, EXPONENT,
    // Binary
    SLASH, LEFT_SHIFT, RIGHT_SHIFT, FLOOR, MODULO,
    PLUS_EQUAL, MINUS_EQUAL, STAR_EQUAL, SLASH_EQUAL, MODULO_EQUAL,
    EXPONENT_EQUAL, FLOOR_EQUAL, EQUAL, BANG_EQUAL, BANG, EQUAL_EQUAL,
    LESS_EQUAL, LESS, GREATER_EQUAL, GREATER,
    AND, OR,

    // Literals
    CHARACTER, FRACTION, INTEGER, STRING, LONG_STRING,
    IDENTIFIER,

    // F_FSTRINGS
    F_STRING_START, F_STRING_TEXT, F_STRING_END,

    // Keywords:
    FALSE, NONE, TRUE, AS, ASSERT, ASYNC, AWAIT, BREAK, CLASS,
    CONTINUE, DEF, DEL, ELIF, ELSE, EXCEPT, FINALLY, FOR, FROM,
    GLOBAL, IF, IMPORT, IN, IS, LAMBDA, NONLOCAL, NOT, PASS,
    RAISE, RETURN, TRY, WHILE, WITH, YIELD,

    // TYPES
    CHAR, DICT, FLOAT, INT, LIST, STR,

    COMMENT,
    UNKNOWN,
    END // End Of File (EOF is reserved)
};


inline bool isLiteral(const TOKENTYPE type) {
    return type >= CHARACTER && type <= STRING;
}

inline bool isType(const TOKENTYPE type) {
    return type >= CHAR && type <= STR;
}

inline bool isOnlyUnaryOperation(const TOKENTYPE type) {
    return type == INVERSE;
}

inline bool isUnaryOperation(const TOKENTYPE type) {
    return type >= INVERSE && type <= PLUS;
}

inline bool isOperation(const TOKENTYPE type) {
    return type >= INVERSE && type <= OR;
}

inline bool isAssignment(const TOKENTYPE type) {
    return type >= PLUS_EQUAL && type <= EQUAL;
}

inline bool isString(const TOKENTYPE type) {
    return type == STRING || type == LONG_STRING;
}


static std::unordered_map<std::string, TOKENTYPE> KEYWORDS = {
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
    {"float",    FLOAT},
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


inline std::string tokenTypeToString(const TOKENTYPE type) {
    switch (type) {
        case LEFT_PAREN:     return "LEFT_PAREN";
        case RIGHT_PAREN:    return "RIGHT_PAREN";
        case LEFT_BRACE:     return "LEFT_BRACE";
        case RIGHT_BRACE:    return "RIGHT_BRACE";
        case LEFT_BRACKET:   return "LEFT_BRACKET";
        case RIGHT_BRACKET:  return "RIGHT_BRACKET";

        case COMMA:          return "COMMA";
        case DOT:            return "DOT";

        case COLON:          return "COLON";
        case ARROW:          return "ARROW";
        case INDENT:         return "INDENT";
        case DEDENT:         return "DEDENT";

        case INVERSE:        return "INVERSE";
        case MINUS:          return "MINUS";
        case PLUS:           return "PLUS";

        case STAR:           return "STAR";
        case EXPONENT:       return "EXPONENT";
        case SLASH:          return "SLASH";
        case BANG_EQUAL:     return "BANG_EQUAL";
        case BANG:           return "BANG";
        case EQUAL_EQUAL:    return "EQUAL_EQUAL";
        case EQUAL:          return "EQUAL";
        case LESS_EQUAL:     return "LESS_EQUAL";
        case LESS:           return "LESS";
        case GREATER_EQUAL:  return "GREATER_EQUAL";
        case GREATER:        return "GREATER";
        case LEFT_SHIFT:     return "LEFT_SHIFT";
        case RIGHT_SHIFT:    return "RIGHT_SHIFT";
        case PLUS_EQUAL:     return "PLUS_EQUAL";
        case MINUS_EQUAL:    return "MINUS_EQUAL";

        case CHARACTER:      return "CHARACTER";
        case STRING:         return "STRING";
        case LONG_STRING:    return "LONG_STRING";
        case INTEGER:        return "INTEGER";
        case FRACTION:       return "FRACTION";
        case IDENTIFIER:     return "IDENTIFIER";

        case F_STRING_START: return "F_STRING_START";
        case F_STRING_TEXT:  return "F_STRING_TEXT";
        case F_STRING_END:   return "F_STRING_END";

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
        case FLOAT:        return "FLOAT";
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
    TOKENTYPE type;
    std::string lexeme;

    explicit Token(const TOKENTYPE type, std::string lexeme)
        : type(type), lexeme(std::move(lexeme)) {}

    [[nodiscard]] friend std::ostream& operator<<(std::ostream& stream, const Token& token) {
        stream << tokenTypeToString(token.type) << " (" << escapeLexeme(token.lexeme) << ")";
        return stream;
    }
};


#endif //PYTHONCOMPILER_TOKEN_H
