#pragma once


namespace ast {

class Addition;
class And;
class Assignment;
class BooleanLiteral;
class Comparison;
class FloatLiteral;
class FunctionCall;
class IfThen;
class IfThenElse;
class IntLiteral;
class Name;
class Or;
class Scope;
class StringLiteral;
class While;

// TODO: const correctness
class Visitor
{
public:
    virtual void visitAddition(const Addition & name) = 0;
    virtual void visitAnd(const And & test) = 0;
    virtual void visitAssignment(const Assignment & assignment) = 0;
    virtual void visitBooleanLiteral(const BooleanLiteral & literal) = 0; // TODO: test literals that do not fit
    virtual void visitComparison(const Comparison & visitable) = 0;
    virtual void visitFloatLiteral(const FloatLiteral & floatLiteral) = 0; // TODO: test literals that do not fit
    virtual void visitFree() = 0;
    virtual void visitFunctionCall(const FunctionCall & functionCall) = 0;
    virtual void visitIfThen(const IfThen & ifThen) = 0;
    virtual void visitIfThenElse(const IfThenElse & ifThenElse) = 0;
    virtual void visitIntLiteral(const IntLiteral & intLiteral) = 0;
    virtual void visitName(const Name & name) = 0;
    virtual void visitOr(const Or & test) = 0;
    virtual void visitScope(const Scope & scope) = 0;
    virtual void visitStringLiteral(const StringLiteral & visitable) = 0;
    virtual void visitWhile(const While & loop) = 0;
};


} // namespace ast
