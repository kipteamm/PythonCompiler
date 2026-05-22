#include "Expression.h"

#include <stdexcept>


void Expression::accept(ASTVisitor *visitor) {
    throw std::runtime_error("Visitor accept function not implemented");
}


Char::Char(const char value) : value(value) {}


Int::Int(const int value) : value(value) {}
