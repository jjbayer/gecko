#include "printvisitor.hpp"
#include "ast.hpp"

#include <unordered_map>


namespace ast {

void PrintVisitor::visitAddition(const Addition & addition)
{
    addition.mLeft->acceptVisitor(*this);
    mOut << " + ";
    addition.mRight->acceptVisitor(*this);
}

void PrintVisitor::visitAnd(const And &visitable)
{
    visitable.mLeft->acceptVisitor(*this);
    mOut << " and ";
    visitable.mRight->acceptVisitor(*this);
}

void PrintVisitor::visitAssignment(const Assignment &assignment)
{
    assignment.mAssignee->acceptVisitor(*this);
    mOut << " = ";
    assignment.mValue->acceptVisitor(*this);
}

void PrintVisitor::visitFunctionCall(const FunctionCall &functionCall)
{
    functionCall.mName->acceptVisitor(*this);

    if( functionCall.mTypeParameters ) {
        functionCall.mTypeParameters->acceptVisitor(*this);
    }

    mOut << "(";
    auto tail = false;
    for(const auto & argument : functionCall.mArguments) {
        if( tail ) mOut << ", ";
        tail = true;
        argument->acceptVisitor(*this);
    }
    mOut << ")";
}


void PrintVisitor::visitFunctionDefinition(const FunctionDefinition & functionDefinition)
{
    mOut << "function ";
    functionDefinition.mName->acceptVisitor(*this);
    mOut << "(";
    auto tail = false;
    for(const auto & argument : functionDefinition.mArguments) {
        if( tail ) mOut << ", ";
        tail = true;
        argument.first->acceptVisitor(*this);
        mOut << ": ";
        argument.second->acceptVisitor(*this);
    }
    mOut << ")";
    mOut << "\n";
    mIndent++;
    functionDefinition.mBody->acceptVisitor(*this);
    mIndent--;
    // TODO: suppress additional newline

}

void PrintVisitor::visitIntLiteral(const IntLiteral &literal)
{
    mOut << literal.mValue;
}

void PrintVisitor::visitFloatLiteral(const FloatLiteral &literal)
{
    std::showpoint(mOut);
    mOut << literal.mValue;
}

void PrintVisitor::visitFor(const For &loop)
{
    mOut << "for ";
    loop.mLoopVariable->acceptVisitor(*this);
    mOut << " in ";
    loop.mRange->acceptVisitor(*this);

    mOut << "\n";
    mIndent++;
    loop.mBody->acceptVisitor(*this);
    mIndent--;
    // TODO: suppress additional newline
}

void PrintVisitor::visitFree()
{
    mOut << "free";
}

void PrintVisitor::visitBooleanLiteral(const BooleanLiteral &literal)
{
    mOut << (literal.mValue ? "true" : "false");
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
        mOut << tokenMap.at(visitable.mOperators.at(i));
    }

    // Last element
    (*visitable.mOperands.rbegin())->acceptVisitor(*this);
}

void PrintVisitor::visitName(const Name &name)
{
    mOut << name.mName;
}

void PrintVisitor::visitTypeName(const TypeName &name)
{
    mOut << name.mName;
}


void PrintVisitor::visitType(const Type &type)
{
    type.mTypeName->acceptVisitor(*this);
    if( auto & params = type.mTypeParameters) {
        params->acceptVisitor(*this);
    }
}


void PrintVisitor::visitOr(const Or &visitable)
{
    visitable.mLeft->acceptVisitor(*this);
    mOut << " or ";
    visitable.mRight->acceptVisitor(*this);
}

void PrintVisitor::visitScope(const Scope &scope)
{
    for(const auto & statement : scope.mStatements) {
        for(int i = 0; i < mIndent; i++) mOut << "    ";
        statement->acceptVisitor(*this);
        mOut << "\n";
    }
}

void PrintVisitor::visitStringLiteral(const StringLiteral &visitable)
{
    mOut << '"' << visitable.mValue << '"';
}

void PrintVisitor::visitWhile(const While &loop)
{
    mOut << "while ";
    loop.mCondition->acceptVisitor(*this);
    mOut << "\n";
    mIndent++;
    loop.mBody->acceptVisitor(*this);
    mIndent--;
    // TODO: suppress additional newline
}

void PrintVisitor::visitIfThen(const IfThen &ifThen)
{
    mOut << "if ";
    ifThen.mCondition->acceptVisitor(*this);
    mOut << "\n";
    mIndent++;
    ifThen.mIfBlock->acceptVisitor(*this);
    mIndent--;
}

void PrintVisitor::visitIfThenElse(const IfThenElse &ifThenElse)
{
    mOut << "if ";
    ifThenElse.mCondition->acceptVisitor(*this);
    mOut << "\n";
    mIndent++;
    ifThenElse.mIfBlock->acceptVisitor(*this);
    mIndent--;
    for(int i = 0; i < mIndent; i++) mOut << "    ";
    mOut << "else\n";
    mIndent++;
    ifThenElse.mElseBlock->acceptVisitor(*this);
    mIndent--;
}


void PrintVisitor::visitTypeParameterList(const TypeParameterList & typeParameters)
{
    mOut << "<";
    auto tail = false;
    for(const auto & param : typeParameters.mTypeParameters) {
        if( tail ) mOut << ", ";
        tail = true;
        param->acceptVisitor(*this);
    }
    mOut << ">";
}


} // namespace ast
