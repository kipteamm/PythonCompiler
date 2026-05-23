#ifndef PYTHONCOMPILER_ASTVISITOR_H
#define PYTHONCOMPILER_ASTVISITOR_H


class Node;
class Scope;
class Comment;
class Assignment;
class Function;
class Parameter;
class Literal;
class Char;
class Int;


class ASTVisitor {
public:
    virtual ~ASTVisitor() = default;
    virtual void visit(Scope* node) = 0;
    virtual void visit(Comment* node) = 0;
    virtual void visit(Assignment* node) = 0;
    virtual void visit(Function* node) = 0;
    virtual void visit(Parameter* node) = 0;
    virtual void visit(Char* node) = 0;
    virtual void visit(Int* node) = 0;
};


#endif //PYTHONCOMPILER_ASTVISITOR_H
