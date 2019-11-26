#include "printvisitor.hpp"
#include "ast.hpp"

#include <iostream>
#include <unordered_map>


namespace ast {

void PrintVisitor::visitAddition(const Addition & addition)
{
    addition.mLeft->acceptVisitor(*this);
    std::cout << " + ";
    addition.mRight->acceptVisitor(*this);
}

void PrintVisitor::visitAnd(const And &visitable)
{
    visitable.mLeft->acceptVisitor(*this);
    std::cout << " and ";
    visitable.mRight->acceptVisitor(*this);
}

void PrintVisitor::visitAssignment(const Assignment &assignment)
{
    assignment.mAssignee->acceptVisitor(*this);
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

void PrintVisitor::visitIntLiteral(const IntLiteral &literal)
{
    std::cout << literal.mValue;
}

void PrintVisitor::visitFloatLiteral(const FloatLiteral &literal)
{
    std::showpoint(std::cout);
    std::cout << literal.mValue;
}

void PrintVisitor::visitFor(const For &loop)
{
    std::cout << "for ";
    loop.mLoopVariable->acceptVisitor(*this);
    std::cout << " in ";
    loop.mRange->acceptVisitor(*this);

    std::cout << "\n";
    mIndent++;
    loop.mBody->acceptVisitor(*this);
    mIndent--;
    // TODO: suppress additional newline
}

void PrintVisitor::visitFree()
{
    std::cout << "free";
}

void PrintVisitor::visitBooleanLiteral(const BooleanLiteral &literal)
{
    std::cout << (literal.mValue ? "true" : "false");
}

void PrintVisitor::visitComparison(const Comparison &visitable)
{
    // TODO: central type to string mapping
    const std::unordered_map<Token::Type, std::string> tokenMap {
            {Token::LessThan, " < "},
            {Token::LTE, " <= "},
            {Token::Equal, " == "},
            {Token::NotEqual, " != "},
            {Token::GTE, " >= "},
            {Token::GreaterThan, " > "}
    };

    for(size_t i = 0; i < visitable.mOperators.size(); i++) {
        visitable.mOperands.at(i)->acceptVisitor(*this);
        std::cout << tokenMap.at(visitable.mOperators.at(i));
    }

    // Last element
    (*visitable.mOperands.rbegin())->acceptVisitor(*this);
}

void PrintVisitor::visitName(const Name &name)
{
    std::cout << name.mName;
}

void PrintVisitor::visitOr(const Or &visitable)
{
    visitable.mLeft->acceptVisitor(*this);
    std::cout << " or ";
    visitable.mRight->acceptVisitor(*this);
}

void PrintVisitor::visitScope(const Scope &scope)
{
    for(const auto & statement : scope.mStatements) {
        for(int i = 0; i < mIndent; i++) std::cout << "    ";
        statement->acceptVisitor(*this);
        std::cout << "\n";
    }
}

void PrintVisitor::visitStringLiteral(const StringLiteral &visitable)
{
    std::cout << '"' << visitable.mValue << '"';
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

void PrintVisitor::visitIfThen(const IfThen &ifThen)
{
    std::cout << "if ";
    ifThen.mCondition->acceptVisitor(*this);
    std::cout << "\n";
    mIndent++;
    ifThen.mIfBlock->acceptVisitor(*this);
    mIndent--;
}

void PrintVisitor::visitIfThenElse(const IfThenElse &ifThenElse)
{
    std::cout << "if ";
    ifThenElse.mCondition->acceptVisitor(*this);
    std::cout << "\n";
    mIndent++;
    ifThenElse.mIfBlock->acceptVisitor(*this);
    mIndent--;
    for(int i = 0; i < mIndent; i++) std::cout << "    ";
    std::cout << "else\n";
    mIndent++;
    ifThenElse.mElseBlock->acceptVisitor(*this);
    mIndent--;
}


} // namespace ast
