#ifndef PYTHONCOMPILER_DOTVISITOR_H
#define PYTHONCOMPILER_DOTVISITOR_H
#include <sstream>
#include <string>

#include "ASTVisitor.h"


class DotVisitor final : public ASTVisitor {
public:
    DotVisitor();

    std::string getDot();

    void visit(Scope *node) override;
    void visit(Comment *node) override;
    void visit(Assignment *node) override;
    void visit(Function *node) override;
    void visit(Parameter* node) override;
    void visit(Return* node) override;
    void visit(Char *node) override;
    void visit(Int *node) override;
    void visit(Float* node) override;

private:
    std::ostringstream oss;
};


#endif //PYTHONCOMPILER_DOTVISITOR_H
