#ifndef PYTHONCOMPILER_DOTVISITOR_H
#define PYTHONCOMPILER_DOTVISITOR_H
#include <sstream>
#include <string>

#include "ASTVisitor.h"


class ASTRenderer final : public ASTVisitor {
public:
    ASTRenderer();

    std::string getDot() const;

    void visit(Scope *node) override;
    void visit(Comment *node) override;
    void visit(Assignment *node) override;
    void visit(Break *node) override;
    void visit(Continue *node) override;
    void visit(Discard *node) override;
    void visit(ForEach* node) override;
    void visit(Function *node) override;
    void visit(Parameter* node) override;
    void visit(If* node) override;
    void visit(Return* node) override;
    void visit(While* node) override;
    void visit(Binary* node) override;
    void visit(Unary* node) override;
    void visit(Dictionary* node) override;
    void visit(FunctionCall *node) override;
    void visit(Identifier* node) override;
    void visit(List *node) override;
    void visit(Bool *node) override;
    void visit(Char *node) override;
    void visit(Float* node) override;
    void visit(Int *node) override;
    void visit(String *node) override;
    void visit(JoinedString* node) override;
    void visit(GenericType* node) override;
    void visit(PrimitiveType* node) override;
    void visit(UnionType* node) override;
    void visit(UnresolvedType* node) override;

private:
    std::ostringstream oss;
};


#endif //PYTHONCOMPILER_DOTVISITOR_H
