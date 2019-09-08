#include "printvisitor.hpp"
#include "ast.hpp"

#include <iostream>


namespace ast {

void PrintVisitor::visitAddition(const Addition & addition)
{
    addition.mLeft->acceptVisitor(*this);
    std::cout << " + ";
    addition.mRight->acceptVisitor(*this);
}

void PrintVisitor::visitAssignment(const Assignment &assignment)
{
    assignment.mName->acceptVisitor(*this);
    std::cout << " = ";
    assignment.mValue->acceptVisitor(*this);
}

void PrintVisitor::visitFunctionCall(const FunctionCall &functionCall)
{
    functionCall.mName->acceptVisitor(*this);
    std::cout << "(";
    auto tail = false;
    for(const auto & argument : functionCall.mArguments) {
        if( tail ) std::cout << ", ";
        tail = true;
        argument->acceptVisitor(*this);
    }
    std::cout << ")";
}

void PrintVisitor::visitIntLiteral(const IntLiteral &intLiteral)
{
    std::cout << intLiteral.mValue;
}

void PrintVisitor::visitName(const Name &name)
{
    std::cout << name.mName;
}

void PrintVisitor::visitScope(const Scope &scope)
{
    for(const auto & statement : scope.mStatements) {
        for(int i = 0; i < mIndent; i++) std::cout << "    ";
        statement->acceptVisitor(*this);
        std::cout << "\n";
    }
}

void PrintVisitor::visitWhile(const While &loop)
{
    std::cout << "while ";
    loop.mCondition->acceptVisitor(*this);
    std::cout << "\n";
    mIndent++;
    loop.mBody->acceptVisitor(*this);
    mIndent--;
    // TODO: suppress additional newline
}

void PrintVisitor::visitLessThan(const LessThan & lessThan)
{
    lessThan.mLeft->acceptVisitor(*this);
    std::cout << " < ";
    lessThan.mRight->acceptVisitor(*this);
}

void PrintVisitor::visitIfThenElse(const IfThenElse &ifThenElse)
{
    std::cout << "if ";
    ifThenElse.mCondition->acceptVisitor(*this);
    mIndent++;
    ifThenElse.mIfBlock->acceptVisitor(*this);
    mIndent--;
    std::cout << "else\n";
    ifThenElse.mElseBlock->acceptVisitor(*this);
}


} // namespace ast
