#pragma once


namespace ast {

class Addition;
class Assignment;
class FunctionCall;
class IntLiteral;
class Name;
class Scope;


class Visitor
{
public:
    virtual void visitAddition(const Addition & name) = 0;
    virtual void visitAssignment(const Assignment & assignment) = 0;
    virtual void visitFunctionCall(const FunctionCall & functionCall) = 0;
    virtual void visitIntLiteral(const IntLiteral & intLiteral) = 0;
    virtual void visitName(const Name & name) = 0;
    virtual void visitScope(const Scope & scope) = 0;

};

} // namespace ast
