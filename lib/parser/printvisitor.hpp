#pragma once
#include "visitor.hpp"


namespace ast { // TODO: does not belong in this namespace

class PrintVisitor: public Visitor
{
public:
    void visitAddition(const Addition &visitable) override;
    void visitAssignment(const Assignment &assignment) override;
    void visitFunctionCall(const FunctionCall &functionCall) override;
    void visitIntLiteral(const IntLiteral &literal) override;
    void visitFloatLiteral(const FloatLiteral & literal) override;
    void visitName(const Name &name) override;
    void visitScope(const Scope &scope) override;
    void visitWhile(const While &loop) override;
    void visitLessThan(const LessThan &lessThan) override;
    // TODO: add visitIf()
    void visitIfThen(const ast::IfThen &ifThen) override;
    void visitIfThenElse(const IfThenElse &ifThenElse) override;

private:
    int mIndent = 0;

};

} // namespace ast
