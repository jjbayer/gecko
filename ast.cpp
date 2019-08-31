#include "ast.hpp"
#include "visitor.hpp"


namespace ast
{

Name::Name(const std::string &&name)
    : mName(name)
{

}

void Name::acceptVisitor(Visitor &visitor)
{
    visitor.visitName(*this);
}

IntLiteral::IntLiteral(int value)
    : mValue(value)
{

}

void IntLiteral::acceptVisitor(Visitor &visitor)
{
    visitor.visitIntLiteral(*this);
}

Assignment::Assignment(std::unique_ptr<Name> &&name, std::unique_ptr<Expression> &&value)
    : mName(std::move(name))
    , mValue(std::move(value))
{
}

void Assignment::acceptVisitor(Visitor &visitor)
{
    visitor.visitAssignment(*this);
}

void Scope::addStatement(std::unique_ptr<Statement> &&statement)
{
    mStatements.push_back(std::move(statement));
}

void Scope::acceptVisitor(Visitor &visitor)
{
    visitor.visitScope(*this); // TODO: indent
}

FunctionCall::FunctionCall(std::unique_ptr<Name> &&name)
    : mName(std::move(name))
{

}

void FunctionCall::addArgument(std::unique_ptr<Expression> &&expression)
{
    mArguments.push_back(std::move(expression));
}

void FunctionCall::acceptVisitor(Visitor &visitor)
{
    visitor.visitFunctionCall(*this);
}

void Addition::acceptVisitor(Visitor &visitor)
{

    visitor.visitAddition(*this);
}

} // namespace ast
