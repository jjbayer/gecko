#include "ast.hpp"
#include "visitor.hpp"


namespace ast
{

Name::Name(const std::string &name)
    : mName(name)
{

}

void Name::acceptVisitor(Visitor &visitor)
{
    visitor.visitName(*this);
}

IntLiteral::IntLiteral(int64_t value)
    : mValue(value)
{

}

void IntLiteral::acceptVisitor(Visitor &visitor)
{
    visitor.visitIntLiteral(*this);
}

Assignment::Assignment(std::unique_ptr<Assignee> &&name, std::unique_ptr<Expression> &&value)
    : mAssignee(std::move(name))
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

Addition::Addition(std::unique_ptr<Expression> &&left, std::unique_ptr<Expression> &&right)
    : mLeft(std::move(left))
    , mRight(std::move(right))
{

}

void Addition::acceptVisitor(Visitor &visitor)
{

    visitor.visitAddition(*this);
}

While::While(std::unique_ptr<Expression> &&condition, std::unique_ptr<Scope> &&body)
    : mCondition(std::move(condition))
    , mBody(std::move(body))
{

}

void While::acceptVisitor(Visitor &visitor)
{
    visitor.visitWhile(*this);
}

LessThan::LessThan(std::unique_ptr<Singular> &&left, std::unique_ptr<Expression> &&right)
    : mLeft(std::move(left))
    , mRight(std::move(right))
{

}

void LessThan::acceptVisitor(Visitor &visitor)
{
    visitor.visitLessThan(*this);
}

IfThenElse::IfThenElse(std::unique_ptr<Expression> &&condition, std::unique_ptr<Scope> &&ifBlock, std::unique_ptr<Scope> &&elseBlock)
    : mCondition(std::move(condition))
    , mIfBlock(std::move(ifBlock))
    , mElseBlock(std::move(elseBlock))
{

}

void IfThenElse::acceptVisitor(Visitor &visitor)
{
    visitor.visitIfThenElse(*this);
}

} // namespace ast
