#ifndef PYTHONCOMPILER_NODE_H
#define PYTHONCOMPILER_NODE_H

#include "../ASTVisitor.h"


class Node {
public:
    Node() = default;
    virtual ~Node() = default;
    
    virtual void accept(ASTVisitor* visitor) = 0;
};


#endif //PYTHONCOMPILER_NODE_H
