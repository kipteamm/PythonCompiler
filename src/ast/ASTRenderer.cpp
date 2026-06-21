#include "ASTRenderer.h"

#include "nodes/Expression.h"
#include "nodes/Statement.h"
#include "nodes/Type.h"


ASTRenderer::ASTRenderer() {
    oss << "digraph AST {\n";
    oss << "\tsplines=true; nodesep=0.5; ranksep=0.5;\n\n\tnode [shape=box, fontname=\"Courier\"];\n";
}


std::string ASTRenderer::getDot() const {
    return oss.str() + "\n}";
}


void ASTRenderer::visit(Scope *node) {
    oss << "\t" << *node << "[label=\"Scope\"];\n";

    for (const auto& stmt : node->getStatements()) {
        oss << "\t" << *node << " -> " << *stmt << ";\n";
        stmt->accept(this);
    }
}


void ASTRenderer::visit(Comment* node) {
    std::string cleanComment = node->getComment();
    size_t pos = 0;

    // Clean up text quotes for DOT format safety
    while((pos = cleanComment.find('"', pos)) != std::string::npos) {
        cleanComment.replace(pos, 1, "\\\"");
        pos += 2;
    }

    oss << "\t" << *node << " [label=\"Comment\\n" << cleanComment << "\", fontcolor=\"#6a737d\"];\n";
}


void ASTRenderer::visit(Assignment* node) {
    oss << "\t" << *node << " [label=\"Assignment " << node->getIdentifier().lexeme << "\", fontcolor=\"#d73a49\"];\n";

    if (node->getType() != nullptr) {
        node->getType()->accept(this);
        oss << "\t" << *node << " -> " << *node->getType() << ";\n";
    }

    if (node->getExpr()) {
        node->getExpr()->accept(this);
        oss << "\t" << *node << " -> " << *node->getExpr() << ";\n";
    }
}


void ASTRenderer::visit(Break* node) {
    oss << "\t" << *node << " [label=\"Break\", fontcolor=\"#d73a49\"];\n";
}


void ASTRenderer::visit(Continue* node) {
    oss << "\t" << *node << " [label=\"Continue\", fontcolor=\"#d73a49\"];\n";
}



void ASTRenderer::visit(Discard* node) {
    oss << "\t" << *node << " [label=\"Discard\", fontcolor=\"#d6d6d6\"];\n";

    node->getExpr()->accept(this);
    oss << "\t" << *node << " -> " << *node->getExpr() << ";\n";
}


void ASTRenderer::visit(ForEach* node) {
    oss << "\t" << *node << " [label=\"For (" << node->getIdentifier().lexeme << ")\", fontcolor=\"#d73a49\"];\n";

    node->getIterable()->accept(this);
    oss << "\t" << *node << " -> " << * node->getIterable() << ";\n";

    node->getBodyScope()->accept(this);
    oss << "\t" << *node << " -> " << * node->getBodyScope() << ";\n";

    if (node->getElseScope() == nullptr) return;

    node->getElseScope()->accept(this);
    oss << "\t" << *node << " -> " << * node->getElseScope() << ";\n";
}



void ASTRenderer::visit(Function* node) {
    oss << "\t" << *node << " [label=\"Function\n" << node->getName().lexeme << "\", fontcolor=\"#d73a49\"];\n";

    node->getReturnType()->accept(this);
    oss << "\t" << *node << " -> " << *node->getReturnType() << ";\n";

    for (const std::unique_ptr<Parameter>& param : node->getParameters()) {
        param->accept(this);
        oss << "\t" << *node << " -> " << *param << ";\n";
    }

    node->getBody()->accept(this);

    oss << "\t" << *node << " -> " << *node->getBody() << ";\n";
}


void ASTRenderer::visit(FunctionCall* node) {
    oss << "\t" << *node << " [label=\"FunctionCall\n" << node->getIdentifier().lexeme << "\", fontcolor=\"#d2a8ff\"];\n";

    for (const std::unique_ptr<Expression>& arg : node->getArguments()) {
        arg->accept(this);
        oss << "\t" << *node << " -> " << *arg << ";\n";
    }
}


void ASTRenderer::visit(If* node) {
    oss << "\t" << *node << " [label=\"If\", fontcolor=\"#d73a49\"];\n";

    node->getCondition()->accept(this);
    oss << "\t" << *node << " -> " << *node->getCondition() << ";\n";

    node->getThenScope()->accept(this);
    oss << "\t" << *node << " -> " << *node->getThenScope() << ";\n";

    if (!node->getElseScope()) return;
    node->getElseScope()->accept(this);
    oss << "\t" << *node << " -> " << *node->getElseScope() << ";\n";
}


void ASTRenderer::visit(Parameter* node) {
    oss << "\t" << *node << " [label=\"Parameter\n" << node->getIdentifier().lexeme << "\", fontcolor=\"#005cc5\"];\n";

    node->getType()->accept(this);
    oss << "\t" << *node << " -> " << *node->getType() << ";\n";

    if (node->getInitValue() == nullptr) return;

    node->getInitValue()->accept(this);
    oss << "\t" << *node << " -> " << *node->getInitValue() << ";\n";
}


void ASTRenderer::visit(Return* node) {
    oss << "\t" << *node << " [label=\"Return\", fontcolor=\"#d73a49\"];\n";

    node->getExpr()->accept(this);

    oss << "\t" << *node << " -> " << *node->getExpr() << ";\n";
}


void ASTRenderer::visit(While* node) {
    oss << "\t" << *node << " [label=\"While\", fontcolor=\"#d73a49\"];\n";

    node->getCondition()->accept(this);
    oss << "\t" << *node << " -> " << * node->getCondition() << ";\n";

    node->getBodyScope()->accept(this);
    oss << "\t" << *node << " -> " << * node->getBodyScope() << ";\n";

    if (node->getElseScope() == nullptr) return;

    node->getElseScope()->accept(this);
    oss << "\t" << *node << " -> " << * node->getElseScope() << ";\n";
}


void ASTRenderer::visit(Binary* node) {
    oss << "\t" << *node << " [label=\"Binary(" << node->getOperation().lexeme << ")\", fontcolor=\"#d73a49\"];\n";

    node->getLhs()->accept(this);
    oss << "\t" << *node << " -> " << *node->getLhs() << ";\n";

    node->getRhs()->accept(this);
    oss << "\t" << *node << " -> " << *node->getRhs() << ";\n";
}

void ASTRenderer::visit(Unary* node) {
    oss << "\t" << *node << " [label=\"Binary(" << node->getOperation().lexeme << ")\", fontcolor=\"#d73a49\"];\n";

    node->getExpr()->accept(this);
    oss << "\t" << *node << " -> " << *node->getExpr() << ";\n";
}


void ASTRenderer::visit(Identifier* node) {
    oss << "\t" << *node << " [label=\"ID(" << node->getName() << ")\"];\n";
}


void ASTRenderer::visit(Bool* node) {
    oss << "\t" << *node << " [label=\"Bool(" << node->getValue() << ")\", fontcolor=\"#005cc5\"];\n";
}


void ASTRenderer::visit(Char* node) {
    oss << "\t" << *node << " [label=\"Char(" << node->getValue() << ")\", fontcolor=\"#005cc5\"];\n";
}


void ASTRenderer::visit(Float* node) {
    oss << "\t" << *node << " [label=\"Float(" << node->getValue() << ")\", fontcolor=\"#005cc5\"];\n";
}


void ASTRenderer::visit(Int* node) {
    oss << "\t" << *node << " [label=\"Int(" << node->getValue() << ")\", fontcolor=\"#005cc5\"];\n";
}


void ASTRenderer::visit(String* node) {
    oss << "\t" << *node << " [label=\"String(" << node->getValue() << ")\", fontcolor=\"#005cc5\"];\n";
}


void ASTRenderer::visit(JoinedString* node) {
    oss << "\t" << *node << " [label=\"JoinedString\", fontcolor=\"#005cc5\"];\n";

    for (const auto &value : node->getValues()) {
        value->accept(this);
        oss << "\t" << *node << " -> " << *value << ";\n";
    }
}


void ASTRenderer::visit(PrimitiveType* node) {
    oss << "\t" << *node << " [label=\"PrimitiveType\\n" << node->getToken().lexeme << "\", fontcolor=\"#005cc5\"];\n";
}


void ASTRenderer::visit(UnionType* node) {
    oss << "\t" << *node << " [label=\"UnionType\", fontcolor=\"#005cc5\"];\n";

    for (const auto& innerType : node->getTypes()) {
        innerType->accept(this);
        oss << "\t" << *node << " -> " << *innerType << ";\n";
    }
}


void ASTRenderer::visit(GenericType* node) {
    oss << "\t" << *node << " [label=\"GenericType\\n" << node->getBase().lexeme << "\", fontcolor=\"#005cc5\"];\n";

    for (const auto& argType : node->getArguments()) {
        argType->accept(this);
        oss << "\t" << *node << " -> " << *argType << ";\n";
    }
}


void ASTRenderer::visit(UnresolvedType* node) {
    oss << "\t" << *node << " [label=\"PrimitiveType\\n" << node->getIdentifier().lexeme << "\", fontcolor=\"#005cc5\"];\n";
}
