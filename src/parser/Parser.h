#ifndef PYTHONCOMPILER_PARSER_H
#define PYTHONCOMPILER_PARSER_H
#include <vector>

#include "../ast/nodes/Expression.h"
#include "../ast/nodes/Statement.h"
#include "../symbol/SymbolTable.h"
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

    Token consume(Assertion assertion, const std::string& error);
    Token consume(TOKENTYPE type, const std::string& error);
    Token advance();

    [[nodiscard]] std::unique_ptr<Scope> scope();

    // STATEMENTS
    [[nodiscard]] std::unique_ptr<Statement> statement(bool global);

    [[nodiscard]] std::unique_ptr<Assignment> assignment();
    [[nodiscard]] std::unique_ptr<Comment> comment();
    [[nodiscard]] std::unique_ptr<Function> function();
    [[nodiscard]] std::unique_ptr<Parameter> parameter();
    [[nodiscard]] std::unique_ptr<Return> return_();

    // IF STATEMENT
    [[nodiscard]] std::unique_ptr<If> if_();

    // EXPRESSIONS
    [[nodiscard]] std::unique_ptr<Expression> expression(std::unique_ptr<Expression> lhs);
    [[nodiscard]] std::unique_ptr<FunctionCall> functionCall(const Token& token);
    [[nodiscard]] std::unique_ptr<Expression> primary();

    // LITERALS
    // [[nodiscard]] std::unique_ptr<Literal> literal(const Token& token) const;

    const std::vector<Token>& tokens;

    bool terminated = false;
    int current = 0;
    int level = 0;
};


#endif //PYTHONCOMPILER_PARSER_H
