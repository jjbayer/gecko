#pragma once
#include "visitor.hpp"


namespace ast { // TODO: does not belong in this namespace

class PrintVisitor: public Visitor
{
public:
    void visitAddition(const Addition &visitable);
    void visitAssignment(const Assignment &assignment);
    void visitFunctionCall(const FunctionCall &functionCall);
    void visitIntLiteral(const IntLiteral &intLiteral);
    void visitName(const Name &name);
    void visitScope(const Scope &scope);
    void visitWhile(const While &loop);
    void visitLessThan(const LessThan &lessThan);
    // TODO: add visitIf()
    void visitIfThenElse(const IfThenElse &ifThenElse);

private:
    int mIndent = 0;

};

} // namespace ast
