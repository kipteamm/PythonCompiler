#ifndef PYTHONCOMPILER_PARSER_H
#define PYTHONCOMPILER_PARSER_H
#include <vector>

#include "../ast/nodes/Expression.h"
#include "../ast/nodes/Statement.h"
#include "../ast/nodes/Type.h"

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
    [[nodiscard]] bool match(Assertion isToken);

    Token consume(Assertion assertion, const std::string& error);
    Token consume(TOKENTYPE type, const std::string& error);
    Token advance();

    [[nodiscard]] std::unique_ptr<Scope> scope();

    // STATEMENTS
    [[nodiscard]] std::unique_ptr<Statement> statement();

    [[nodiscard]] std::unique_ptr<Assignment> assignment();
    [[nodiscard]] std::unique_ptr<Function> function();
    [[nodiscard]] std::unique_ptr<If> if_();
    [[nodiscard]] std::unique_ptr<Parameter> parameter();
    [[nodiscard]] std::unique_ptr<Return> return_();
    [[nodiscard]] std::unique_ptr<While> while_();
    [[nodiscard]] std::unique_ptr<ForEach> for_();

    // TYPES
    [[nodiscard]] std::unique_ptr<Type> type();
    [[nodiscard]] std::unique_ptr<Type> singleType();

    // EXPRESSIONS
    [[nodiscard]] std::unique_ptr<Expression> expression();
    [[nodiscard]] std::unique_ptr<Expression> expression_(std::unique_ptr<Expression> lhs);
    [[nodiscard]] std::unique_ptr<FunctionCall> functionCall(const Token& token, std::vector<std::unique_ptr<Type>> typeArguments);
    [[nodiscard]] std::unique_ptr<Expression> primary();
    [[nodiscard]] std::unique_ptr<JoinedString> fString();
    void keyValue(std::vector<std::unique_ptr<Expression>>& keys, std::vector<std::unique_ptr<Expression>>& values);

    const std::vector<Token>& tokens;

    bool terminated = false;
    int current = 0;
    int level = 0;
};


#endif //PYTHONCOMPILER_PARSER_H
