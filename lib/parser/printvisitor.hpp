#pragma once
#include "visitor.hpp"
#include <ostream>

namespace ast { // TODO: does not belong in this namespace

class PrintVisitor: public Visitor
{
public:

    PrintVisitor(std::ostream & output): mOut(output) {}

    void visitAddition(const Addition &visitable) override;
    void visitAnd(const And &visitable) override;
    void visitAssignment(const Assignment &assignment) override;
    void visitBooleanLiteral(const BooleanLiteral &literal) override;
    void visitComparison(const Comparison &visitable) override;
    void visitFloatLiteral(const FloatLiteral & literal) override;
    void visitFor(const For & loop) override;
    void visitFree() override;
    void visitFunctionCall(const FunctionCall &functionCall) override;
    void visitFunctionDefinition(const FunctionDefinition & functionDefinition) override;
    void visitIfThen(const ast::IfThen &ifThen) override;
    void visitIfThenElse(const IfThenElse &ifThenElse) override;
    void visitIntLiteral(const IntLiteral &literal) override;
    void visitName(const Name &name) override;
    void visitOr(const Or & visitable) override;
    void visitScope(const Scope &scope) override;
    void visitStringLiteral(const StringLiteral & visitable) override;
    void visitTypeName(const TypeName & visitable) override;
    void visitType(const Type & visitable) override;
    void visitTypeParameterList(const TypeParameterList & typeParameters) override;
    void visitWhile(const While &loop) override;

private:
    std::ostream & mOut;
    int mIndent = 0;

    // Visitor interface
public:
};

} // namespace ast
