#pragma once
#include "visitor.hpp"


namespace ast {

class PrintVisitor: public Visitor
{
    void visitAddition(const Addition &visitable);
    void visitAssignment(const Assignment &assignment);
    void visitFunctionCall(const FunctionCall &functionCall);
    void visitIntLiteral(const IntLiteral &intLiteral);
    void visitName(const Name &name);
    void visitScope(const Scope &scope);
};

} // namespace ast
