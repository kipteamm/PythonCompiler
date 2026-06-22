#include "Parser.h"

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


std::unique_ptr<Statement> Parser::statement() {
    switch (peek().type) {
        case IDENTIFIER: {
            // This could be either a standalone expression, or a variable
            // assignment/declaration. Depends on what follows, eg;

            if (isAssignment(tokens.at(current + 1).type) || tokens.at(current + 1).type == COLON) {
                // Both declarations and assignments are rather ambigious in Python, so
                // we assume everything is an assignment. This is later properly
                // handled to assure that the first assignment is also a declaration.
                return assignment();
            }

            // Global expressions are technically discarded expressions, tho
            // FunctionCalls do still have effect on the program
            auto expr = expression();
            return std::make_unique<Discard>(std::move(expr));
        }

        case DEF:         return function();
        case IF:          return if_();
        case RETURN:      return return_();
        case WHILE:       return while_();
        case FOR:         return for_();

        case BREAK:
            advance(); return std::make_unique<Break>();
        case CONTINUE:
            advance(); return std::make_unique<Continue>();

        default: {
            // Anything that doesn't match the specific cases here is assumed
            // to be part of an expression.
            auto value = expression();
            return std::make_unique<Discard>(std::move(value));
        }
    }
}


std::pair<TOKENTYPE, std::string> getBaseOperator(const TOKENTYPE compoundAssignment) {
    switch (compoundAssignment) {
        case PLUS_EQUAL:   return {PLUS, "+"};
        case MINUS_EQUAL:  return {MINUS, "-"};
        case MODULO_EQUAL: return {MODULO, "%"};
        case STAR_EQUAL:   return {STAR, "*"};
        case SLASH_EQUAL:  return {SLASH, "/"};
        default:
            throw std::runtime_error("unknown compound operator " + tokenTypeToString(compoundAssignment));
    }
}


std::unique_ptr<Assignment> Parser::assignment() {
    const Token identifier = consume(IDENTIFIER, "expected identifier");
    std::unique_ptr<Type> type = match(COLON)
        ? this->type()
        : nullptr;

    std::unique_ptr<Expression> expr = nullptr;
    TOKENTYPE assignmentType = UNKNOWN;

    if (isAssignment(peek().type)) {
        assignmentType = advance().type;
        expr = expression();
    }

    // Compound assignment
    if (assignmentType != EQUAL) {
        const auto [token, lexeme] = getBaseOperator(assignmentType);
        expr = std::make_unique<Binary>(
            std::make_unique<Identifier>(identifier.lexeme),
            Token{token, lexeme},
            std::move(expr)
        );
    }

    return std::make_unique<Assignment>(identifier, std::move(type), std::move(expr));
}


std::unique_ptr<Function> Parser::function() {
    advance(); // DEF keyword

    const Token& identifier = consume(IDENTIFIER, "expected function name");

    // Generic function with type parameters
    std::vector<std::unique_ptr<TypeParameter>> typeParameters;

    if (match(LEFT_BRACKET)) {
        do {
            Token typeName = consume(IDENTIFIER, "expected type parameter name");

            std::unique_ptr<Type> bound = nullptr;
            if (match(COLON))
                bound = type();

            typeParameters.push_back(std::make_unique<TypeParameter>(typeName, std::move(bound)));
        } while (match(COMMA));

        consume(RIGHT_BRACKET, "expected ']' after generic type parameters");
    }

    consume(LEFT_PAREN, "expected '(' after function name (and optional type parameters)");

    std::vector<std::unique_ptr<Parameter>> parameters;

    while (!match(RIGHT_PAREN)) {
        parameters.push_back(parameter());

        if (peek().type == RIGHT_PAREN) continue;
        consume(COMMA, "Expected next argument");
    }

    consume(ARROW, "expected '->' return type specifier");
    std::unique_ptr<Type> returnType = type();

    consume(COLON, "expected ':' after function signature");

    auto scope = this->scope();

    return std::make_unique<Function>(identifier, std::move(typeParameters), std::move(returnType), std::move(parameters), std::move(scope));
}


std::unique_ptr<If> Parser::if_() {
    advance(); // IF keyword

    auto condition = expression();
    consume(COLON, "expected ':'");

    auto thenScope = scope();
    std::unique_ptr<Scope> elseScope = nullptr;

    if (match(ELSE)) {
        consume(COLON, "expected ':'");
        elseScope = scope();
    } else if (peek().type == ELIF) {
        elseScope = std::make_unique<Scope>();

        auto elif = if_();
        elseScope->addStatement(std::move(elif));
    }

    return std::make_unique<If>(std::move(condition), std::move(thenScope), std::move(elseScope));
}


std::unique_ptr<Parameter> Parser::parameter() {
    const Token& identifier = consume(IDENTIFIER, "expected parameter name");
    consume(COLON, "expected ':' after paremeter name");
    std::unique_ptr<Type> type = this->type();

    std::unique_ptr<Expression> expr = nullptr;
    if (match(EQUAL)) expr = expression();

    return std::make_unique<Parameter>(identifier, std::move(type), std::move(expr));
}


std::unique_ptr<Return> Parser::return_() {
    advance(); // RETURN

    auto expr = expression();

    return std::make_unique<Return>(std::move(expr));
}


std::unique_ptr<While> Parser::while_() {
    advance(); // WHILE

    auto condition = expression();
    consume(COLON, "expected ':'");

    auto bodyScope = scope();

    // Python loops can be chained with an else statement which will be
    // executed after a full and successful iteration (no errors)
    std::unique_ptr<Scope> elseScope = nullptr;
    if (match(ELSE)) {
        consume(COLON, "expected ':'");
        elseScope = scope();
    }

    return std::make_unique<While>(std::move(condition), std::move(bodyScope), std::move(elseScope));
}


std::unique_ptr<ForEach> Parser::for_() {
    advance(); // FOR

    const Token identifier = consume(IDENTIFIER, "missing for identifier");
    consume(IN, "expected 'in' keyword");

    auto iterable = expression();
    consume(COLON, "expected ':'");

    auto bodyScope = scope();

    // Python loops can be chained with an else statement which will be
    // executed after a full and successful iteration (no errors)
    std::unique_ptr<Scope> elseScope = nullptr;
    if (match(ELSE)) {
        consume(COLON, "expected ':'");
        elseScope = scope();
    }

    return std::make_unique<ForEach>(identifier, std::move(iterable), std::move(bodyScope), std::move(elseScope));
}


std::unique_ptr<Type> Parser::type() {
    std::vector<std::unique_ptr<Type>> types;
    types.push_back(singleType());

    // Consume singleTypes for as long as this is a valid union
    while (match(PIPE)) {
        types.push_back(singleType());
    }

    // If it wasn't an union return initial type only
    if (types.size() == 1)
        return std::move(types[0]);

    return std::make_unique<UnionType>(std::move(types));
}

std::unique_ptr<Type> Parser::singleType() {
    Token baseToken = peek();
    std::unique_ptr<Type> baseType;

    switch (baseToken.type) {
        case NONE:
            baseType = std::make_unique<PrimitiveType>(baseToken);
            advance(); // consume the type
            break;

        case IDENTIFIER:
            baseType = std::make_unique<UnresolvedType>(baseToken);
            advance(); // consume the type
            break;

        default:
            throw std::runtime_error("unexpected type " + baseToken.lexeme);
    }

    // Check whether this is a Generic type, if not return early
    if (!match(LEFT_BRACKET))
        return baseType;

    // Generic without arguments
    if (match(RIGHT_BRACKET))
        return std::make_unique<GenericType>(baseToken);

    std::vector<std::unique_ptr<Type>> arguments;
    arguments.push_back(type());

    while (match(COMMA)) {
        arguments.push_back(type());
    }
    consume(RIGHT_BRACKET, "expected ']' after generic type arguments");

    return std::make_unique<GenericType>(baseToken, std::move(arguments));
}


std::unique_ptr<Expression> Parser::expression() {
    return std::move(expression_(std::move(primary())));
}


std::unique_ptr<Expression> Parser::expression_(std::unique_ptr<Expression> lhs) {
    // Just primaries no operations
    if (!isOperation(peek().type) && lhs != nullptr) return lhs;

    Token operation = consume(isOperation, "expected operator");
    auto rhs = primary();

    std::unique_ptr<Expression> expr;
    if (lhs == nullptr) {
        if (!isUnaryOperation(operation.type))
            throw std::runtime_error("not a unary operation");

        expr = std::make_unique<Unary>(std::move(operation), std::move(rhs));
    } else if (rhs == nullptr) {
        throw std::runtime_error("invalid syntax");
    } else {
        if (isOnlyUnaryOperation(operation.type))
            throw std::runtime_error("unary operation expressed as binary operation");

        expr = std::make_unique<Binary>(std::move(lhs), std::move(operation), std::move(rhs));
    }

    return expression_(std::move(expr));
}


std::unique_ptr<FunctionCall> Parser::functionCall(const Token& token, std::vector<std::unique_ptr<Type>> typeArguments) {
    std::vector<std::unique_ptr<Expression>> arguments;

    while (!match(RIGHT_PAREN)) {
        arguments.push_back(expression());

        if (peek().type == RIGHT_PAREN) continue;
        consume(COMMA, "Expected next argument");
    }

    return std::make_unique<FunctionCall>(token, std::move(typeArguments), std::move(arguments));
}


std::unique_ptr<Expression> Parser::primary() {
    switch (peek().type) {
        case IDENTIFIER: {
            // Identifier literals can either be a
            //  - a function call: the token is followed by a '('
            //  - a function call: the token is followed by a '[' -> explicit
            //    generic arguments
            //  - variable identifier: the token is not followed by anuthing of
            //    signficicance
            const auto identifier = advance();

            // Check for explicit generic type arguments
            std::vector<std::unique_ptr<Type>> typeArgs = {};

            if (match(LEFT_BRACKET)) {
                do {
                    typeArgs.push_back(type());
                } while (match(COMMA));

                consume(RIGHT_BRACKET, "expected ']' after type arguments");
            }

            if (match(LEFT_PAREN))
                return functionCall(identifier, std::move(typeArgs));

            return std::make_unique<Identifier>(identifier.lexeme);
        }

        case FALSE:
        case TRUE:           return std::make_unique<Bool>(advance().type == TRUE);

        case CHARACTER:      return std::make_unique<Char>(advance().lexeme[0]);
        case FRACTION:       return std::make_unique<Float>(std::stof(advance().lexeme));
        case INTEGER:        return std::make_unique<Int>(std::stoi(advance().lexeme));

        case STRING:
        case LONG_STRING:    return std::make_unique<String>(std::move(advance().lexeme));

        case F_STRING_START: return fString();

        // '(' expression ')'
        case LEFT_PAREN: {
            advance(); // (
            auto expr = expression();
            consume(RIGHT_PAREN, "missing closing bracket");

            return expr;
        }

        // List
        case LEFT_BRACKET: {
            advance(); // [

            // Empty list
            if (match(RIGHT_BRACKET))
                return std::make_unique<List>();

            std::vector<std::unique_ptr<Expression>> values;
            values.push_back(expression());

            while (match(COMMA)) {
                values.push_back(expression());
            }

            consume(RIGHT_BRACKET, "list not closed");
            return std::make_unique<List>(std::move(values));
        }

        // Dictionary
        case LEFT_BRACE: {
            advance(); // {

            // Empty dict
            if (match(RIGHT_BRACE))
                return std::make_unique<Dictionary>();

            std::vector<std::unique_ptr<Expression>> keys;
            std::vector<std::unique_ptr<Expression>> values;

            keyValue(keys, values);

            while (match(COMMA)) {
                keyValue(keys, values);
            }

            consume(RIGHT_BRACE, "dictionary not closed");
            return std::make_unique<Dictionary>(std::move(keys), std::move(values));
        }

        default: return nullptr;
    }
}


std::unique_ptr<JoinedString> Parser::fString() {
    consume(F_STRING_START, "");
    std::vector<std::unique_ptr<Expression>> values;

    while (!match(F_STRING_END)) {
        std::unique_ptr<Expression> value;
        auto next = peek();

        if (next.type == F_STRING_TEXT) {
            values.push_back(std::make_unique<String>(next.lexeme));
        } else {
            consume(LEFT_BRACE, "");
            values.push_back(std::move(primary()));
            consume(RIGHT_BRACE, "");
        }

        advance();
    }

    return std::make_unique<JoinedString>(std::move(values));
}


void Parser::keyValue(std::vector<std::unique_ptr<Expression>>& keys, std::vector<std::unique_ptr<Expression>>& values) {
    keys.push_back(expression());
    consume(COLON, "':' expected after dictionary key");
    values.push_back(expression());
}
