#include "Parser.h"


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
        case MATCH:       return match_();

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
            Token{token, lexeme, -1},
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


std::unique_ptr<Match> Parser::match_() {
    advance(); // match

    auto expr = expression();

    consume(COLON, "expected ':'");

    std::vector<std::unique_ptr<Case>> cases;

    consume(INDENT, "expected an indented block after 'match' statement");

    // Case consumes the CASE, condition and the body so after that, if it is
    // followed by another CASE, peek() would indeed be of type CASE again.
    while (peek().type == CASE) {
        auto case_ = this->case_();
        cases.push_back(std::move(case_));
    }

    if (cases.size() == 0)
        throw new std::runtime_error("invalid syntax");

    consume(DEDENT, "not sure tbh");

    return std::make_unique<Match>(std::move(expr), std::move(cases));
}


std::unique_ptr<Case> Parser::case_() {
    advance(); // case

    auto pattern = casePattern();

    std::unique_ptr<Expression> guard = nullptr;
    if (match(IF))
        guard = expression();

    consume(COLON, "expected ':' after case pattern");
    auto body = scope();

    return std::make_unique<Case>(std::move(pattern), std::move(guard), std::move(body));
}
