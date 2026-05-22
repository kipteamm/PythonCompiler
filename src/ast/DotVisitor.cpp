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
    const std::string id = nodeId(node);
    oss << "\t" << id << "[label=\"Scope\"];\n";

    for (const auto& statement : node->getStatements()) {
        oss << "\t" << id << " -> " << nodeId(statement.get()) << ";\n";
        statement->accept(this);
    }
}


void DotVisitor::visit(Assignment* node) {
    const std::string id = nodeId(node);

    std::string label = "Assignment\n" + node->getIdentifier().lexeme;
    if (node->getType().type != UNKNOWN) {
        label += ": " + node->getType().lexeme;
    }

    oss << "\t" << id << " [label=\"" << label << "\", fontcolor=\"#d73a49\"];\n";

    if (node->getExpr()) {
        oss << "\t" << id << " -> " << nodeId(node->getExpr()) << ";\n";
        node->getExpr()->accept(this);
    }
}


void DotVisitor::visit(Comment* node) {
    const std::string id = nodeId(node);

    std::string cleanComment = node->getComment();
    size_t pos = 0;

    // Clean up text quotes for DOT format safety
    while((pos = cleanComment.find('"', pos)) != std::string::npos) {
        cleanComment.replace(pos, 1, "\\\"");
        pos += 2;
    }

    oss << "\t" << id << " [label=\"Comment\\n" << cleanComment << "\", fontcolor=\"#6a737d\"];\n";
}


void DotVisitor::visit(Int* node) {
    const std::string id = nodeId(node);
    oss << "\t" << id << " [label=\"Int(" << node->getValue() << ")\", fontcolor=\"#005cc5\"];\n";
}

void DotVisitor::visit(Char* node) {
    const std::string id = nodeId(node);
    oss << "\t" << id << " [label=\"Char(" << node->getValue() << ")\", fontcolor=\"#005cc5\"];\n";
}

void DotVisitor::visit(Function* node) {
    const std::string id = nodeId(node);
    oss << "\t" << id << " [label=\"Function\", fontcolor=purple];\n";
}


std::string DotVisitor::nodeId(Node* node) {
    std::ostringstream address;
    address << "node_" << reinterpret_cast<uintptr_t>(node);
    return address.str();
}
