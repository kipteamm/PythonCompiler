#ifndef PYTHONCOMPILER_PARSER_H
#define PYTHONCOMPILER_PARSER_H
#include <vector>

#include "../ast/nodes/Statement.h"
#include "../common/Token.h"


using Assertion = bool(*)(TOKENTYPE);


// A Recursive Descent parser
class Parser {
public:
    explicit Parser(const std::vector<Token>& tokens);

    [[nodiscard]] std::unique_ptr<Scope> start();

private:
    [[nodiscard]] Token peek() const;
    [[nodiscard]] bool match(TOKENTYPE token);

    Token advanceAssert(Assertion assertion);
    Token advance();

    // STATEMENTS
    [[nodiscard]] std::unique_ptr<Statement> statement();

    [[nodiscard]] std::unique_ptr<Assignment> assignment();
    [[nodiscard]] std::unique_ptr<Comment> comment();
    [[nodiscard]] std::unique_ptr<Function> function();

    // EXPRESSIONS
    [[nodiscard]] std::unique_ptr<Expression> expression();

    // LITERALS
    [[nodiscard]] std::unique_ptr<Literal> literal(const Token& token) const;

    const std::vector<Token>& tokens;

    int current = 0;
};


#endif //PYTHONCOMPILER_PARSER_H
