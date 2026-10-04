#ifndef PYTHONCOMPILER_PATTERN_H
#define PYTHONCOMPILER_PATTERN_H

#include <memory>
#include <vector>

#include "Expression.h"
#include "Node.h"


// case x:
// binds value to variable name
class CapturePattern final : public Pattern {
public:
    explicit CapturePattern(Token identifier);

    void accept(ASTVisitor* visitor) override { visitor->visit(this); }
    [[nodiscard]] Token& getIdentifier() { return identifier; }

private:
    Token identifier;
};


// case 42:, case "hello":
class LiteralPattern final : public Pattern {
public:
    explicit LiteralPattern(std::unique_ptr<Literal> literal);

    void accept(ASTVisitor* visitor) override { visitor->visit(this); }
    [[nodiscard]] Literal* getLiteral() const { return literal.get(); }

private:
    std::unique_ptr<Literal> literal;
};


// case 1 | 2 | 3:
class OrPattern final : public Pattern {
public:
    explicit OrPattern(std::vector<std::unique_ptr<Pattern>> options);

    void accept(ASTVisitor* visitor) override { visitor->visit(this); }
    [[nodiscard]] const std::vector<std::unique_ptr<Pattern>>& getOptions() const { return options; }

private:
    std::vector<std::unique_ptr<Pattern>> options;
};


// case [a, b, *rest]:
class SequencePattern final : public Pattern {
public:
    explicit SequencePattern(std::vector<std::unique_ptr<Pattern>> elements);

    void accept(ASTVisitor* visitor) override { visitor->visit(this); }
    [[nodiscard]] const std::vector<std::unique_ptr<Pattern>>& getElements() const { return elements; }

private:
    std::vector<std::unique_ptr<Pattern>> elements;
};


// case _:
// like capture pattern except _ does not become available in the scope
class WildcardPattern final : public Pattern {
public:
    WildcardPattern() = default;

    void accept(ASTVisitor* visitor) override { visitor->visit(this); }
};


// case Point(x, y):
// class ClassPattern final : public Pattern {
// public:
//     explicit ClassPattern(Token className, std::vector<std::unique_ptr<Pattern>> positionalPatterns);
//
//     void accept(ASTVisitor* visitor) override { visitor->visit(this); }
//     [[nodiscard]] Token& getClassName() { return className; }
//     [[nodiscard]] const std::vector<std::unique_ptr<Pattern>>& getPositionalPatterns() const { return positionalPatterns; }
//
// private:
//     Token className;
//     std::vector<std::unique_ptr<Pattern>> positionalPatterns;
// };


#endif // PYTHONCOMPILER_PATTERN_H