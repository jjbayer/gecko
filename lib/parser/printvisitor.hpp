#pragma once
#include "visitor.hpp"


namespace ast { // TODO: does not belong in this namespace

class PrintVisitor: public Visitor
{
public:
    void visitAddition(const Addition &visitable) override;
    void visitAnd(const And &visitable) override;
    void visitAssignment(const Assignment &assignment) override;
    void visitBooleanLiteral(const BooleanLiteral &literal);
    void visitFloatLiteral(const FloatLiteral & literal) override;
    void visitFunctionCall(const FunctionCall &functionCall) override;
    void visitIfThen(const ast::IfThen &ifThen) override;
    void visitIfThenElse(const IfThenElse &ifThenElse) override;
    void visitIntLiteral(const IntLiteral &literal) override;
    void visitLessThan(const LessThan &lessThan) override;
    void visitName(const Name &name) override;
    void visitOr(const Or & visitable) override;
    void visitScope(const Scope &scope) override;
    void visitWhile(const While &loop) override;

private:
    int mIndent = 0;


    // Visitor interface
public:
};

} // namespace ast
