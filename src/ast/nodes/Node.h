#ifndef PYTHONCOMPILER_NODE_H
#define PYTHONCOMPILER_NODE_H

#include <memory>

#include "../../common/Token.h"
#include "../ASTVisitor.h"


class Node {
public:
    Node() : id(nodeId++) {};
    virtual ~Node() = default;

    virtual void accept(ASTVisitor* visitor) = 0;

    friend std::ostream& operator<<(std::ostream& stream, const Node& node) {
        return stream << "node_" << node.id;
    }

private:
    int id;

    inline static int nodeId = 0;
};


class Expression : public Node {
public:
    Expression() = default;

    void accept(ASTVisitor *visitor) override = 0;
};


class Parameter final : public Node {
public:
    explicit Parameter(Token type, Token identifier, std::unique_ptr<Expression> initValue);

    void accept(ASTVisitor* visitor) override { visitor->visit(this); };

    [[nodiscard]] Token& getType() { return type; }
    [[nodiscard]] Token& getIdentifier() { return identifier; }
    [[nodiscard]] Expression* getInitValue() const { return initValue.get(); }

private:
    Token type;
    Token identifier;
    std::unique_ptr<Expression> initValue;
};


#endif //PYTHONCOMPILER_NODE_H
