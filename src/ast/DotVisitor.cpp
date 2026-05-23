#include "DotVisitor.h"

#include "nodes/Statement.h"


DotVisitor::DotVisitor() {
    oss << "digraph AST {\n";
    oss << "\tsplines=true; nodesep=0.5; ranksep=0.5;\n\n\tnode [shape=box, fontname=\"Courier\"];\n";
}


std::string DotVisitor::getDot() {
    return oss.str() + "\n} ";
}


void DotVisitor::visit(Scope *node) {
    oss << "\t" << *node << "[label=\"Scope\"];\n";

    for (const auto& stmt : node->getStatements()) {
        oss << "\t" << *node << " -> " << *stmt << ";\n";
        stmt->accept(this);
    }
}


void DotVisitor::visit(Assignment* node) {
    std::string label = "Assignment\n" + node->getIdentifier().lexeme;
    if (node->getType().type != UNKNOWN) {
        label += ": " + node->getType().lexeme;
    }

    oss << "\t" << *node << " [label=\"" << label << "\", fontcolor=\"#d73a49\"];\n";

    if (node->getExpr()) {
        oss << "\t" << *node << " -> " << *node->getExpr() << ";\n";
        node->getExpr()->accept(this);
    }
}


void DotVisitor::visit(Comment* node) {
    std::string cleanComment = node->getComment();
    size_t pos = 0;

    // Clean up text quotes for DOT format safety
    while((pos = cleanComment.find('"', pos)) != std::string::npos) {
        cleanComment.replace(pos, 1, "\\\"");
        pos += 2;
    }

    oss << "\t" << *node << " [label=\"Comment\\n" << cleanComment << "\", fontcolor=\"#6a737d\"];\n";
}


void DotVisitor::visit(Int* node) {
    oss << "\t" << *node << " [label=\"Int(" << node->getValue() << ")\", fontcolor=\"#005cc5\"];\n";
}

void DotVisitor::visit(Char* node) {
    oss << "\t" << *node << " [label=\"Char(" << node->getValue() << ")\", fontcolor=\"#005cc5\"];\n";
}

void DotVisitor::visit(Function* node) {
    oss << "\t" << *node << " [label=\"Function\n" << node->getName().lexeme << " -> " << node->getReturnType().lexeme << "\", fontcolor=\"#d73a49\"];\n";

    for (const std::unique_ptr<Parameter>& param : node->getParameters()) {
        param->accept(this);
        oss << "\t" << *node << " -> " << *param << ";\n";
    }
}


void DotVisitor::visit(Parameter* node) {
    oss << "\t" << *node << " [label=\"Parameter\n" << node->getIdentifier().lexeme << ": " << node->getType().lexeme << "\", fontcolor=\"#005cc5\"];\n";

    if (node->getInitValue() == nullptr) return;

    node->getInitValue()->accept(this);
    oss << "\t" << *node << " -> " << *node->getInitValue() << ";\n";
}


