#ifndef PYTHONCOMPILER_ASTVISITOR_H
#define PYTHONCOMPILER_ASTVISITOR_H


class Node;
class Scope;
class Comment;
class Assignment;
class Discard;
class ForEach;
class Function;
class If;
class Parameter;
class Return;
class While;
class Binary;
class Unary;
class Identifier;
class FunctionCall;
class Bool;
class Char;
class Float;
class Int;
class String;
class JoinedString;


class ASTVisitor {
public:
    virtual ~ASTVisitor() = default;
    virtual void visit(Scope* node) = 0;
    virtual void visit(Comment* node) = 0;
    virtual void visit(Assignment* node) = 0;
    virtual void visit(Discard* node) = 0;
    virtual void visit(ForEach* node) = 0;
    virtual void visit(Function* node) = 0;
    virtual void visit(If* node) = 0;
    virtual void visit(Parameter* node) = 0;
    virtual void visit(Return* node) = 0;
    virtual void visit(While* node) = 0;
    virtual void visit(Binary* node) = 0;
    virtual void visit(Unary* node) = 0;
    virtual void visit(Identifier* node) = 0;
    virtual void visit(FunctionCall* node) = 0;
    virtual void visit(Bool* node) = 0;
    virtual void visit(Char* node) = 0;
    virtual void visit(Float* node) = 0;
    virtual void visit(Int* node) = 0;
    virtual void visit(String* node) = 0;
    virtual void visit(JoinedString* node) = 0;
};


#endif //PYTHONCOMPILER_ASTVISITOR_H
