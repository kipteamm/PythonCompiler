#ifndef PYTHONCOMPILER_ASTVISITOR_H
#define PYTHONCOMPILER_ASTVISITOR_H


class Node;
class Scope;
class Comment;
class CapturePattern;
class LiteralPattern;
class OrPattern;
class SequencePattern;
class WildcardPattern;
class Assignment;
class Break;
class Case;
class Continue;
class Discard;
class ForEach;
class Function;
class If;
class Match;
class Parameter;
class Return;
class While;
class Binary;
class Unary;
class Dictionary;
class FunctionCall;
class Identifier;
class List;
class Bool;
class Char;
class Float;
class Int;
class String;
class JoinedString;
class GenericType;
class PrimitiveType;
class TypeParameter;
class UnionType;
class UnresolvedType;


class ASTVisitor {
public:
    virtual ~ASTVisitor() = default;
    virtual void visit(Scope* node) = 0;
    virtual void visit(Comment* node) = 0;
    virtual void visit(Assignment* node) = 0;
    virtual void visit(CapturePattern* node) = 0;
    virtual void visit(LiteralPattern* node) = 0;
    virtual void visit(OrPattern* node) = 0;
    virtual void visit(SequencePattern* node) = 0;
    virtual void visit(WildcardPattern* node) = 0;
    virtual void visit(Break* node) = 0;
    virtual void visit(Case* node) = 0;
    virtual void visit(Continue* node) = 0;
    virtual void visit(Discard* node) = 0;
    virtual void visit(ForEach* node) = 0;
    virtual void visit(Function* node) = 0;
    virtual void visit(If* node) = 0;
    virtual void visit(Match* node) = 0;
    virtual void visit(Parameter* node) = 0;
    virtual void visit(Return* node) = 0;
    virtual void visit(While* node) = 0;
    virtual void visit(Binary* node) = 0;
    virtual void visit(Unary* node) = 0;
    virtual void visit(Dictionary* node) = 0;
    virtual void visit(FunctionCall* node) = 0;
    virtual void visit(Identifier* node) = 0;
    virtual void visit(List* node) = 0;
    virtual void visit(Bool* node) = 0;
    virtual void visit(Char* node) = 0;
    virtual void visit(Float* node) = 0;
    virtual void visit(Int* node) = 0;
    virtual void visit(String* node) = 0;
    virtual void visit(JoinedString* node) = 0;
    virtual void visit(GenericType* node) = 0;
    virtual void visit(PrimitiveType* node) = 0;
    virtual void visit(TypeParameter* node) = 0;
    virtual void visit(UnionType* node) = 0;
    virtual void visit(UnresolvedType* node) = 0;
};


#endif //PYTHONCOMPILER_ASTVISITOR_H
