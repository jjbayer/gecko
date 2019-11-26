#include "ast.hpp"
#include "visitor.hpp"


namespace ast
{

Name::Name(const std::string &name, const Position &position)
    : Assignee(position)
    , mName(name)
{

}

void Name::acceptVisitor(Visitor &visitor)
{
    visitor.visitName(*this);
}

IntLiteral::IntLiteral(int64_t value, const Position & position)
    : Singular(position)
    , mValue(value)
{

}

void IntLiteral::acceptVisitor(Visitor &visitor)
{
    visitor.visitIntLiteral(*this);
}

Assignment::Assignment(std::unique_ptr<Assignee> &&name, std::unique_ptr<Expression> &&value, const Position &position)
    : Statement(position)
    , mAssignee(std::move(name))
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

FunctionCall::FunctionCall(std::unique_ptr<Name> &&name, const Position &position)
    : Singular(position)
    , mName(std::move(name))
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

Addition::Addition(std::unique_ptr<Expression> &&left, std::unique_ptr<Expression> &&right, const Position &position)
    : Expression(position)
    , mLeft(std::move(left))
    , mRight(std::move(right))
{

}

void Addition::acceptVisitor(Visitor &visitor)
{

    visitor.visitAddition(*this);
}

While::While(std::unique_ptr<Expression> &&condition, std::unique_ptr<Scope> &&body, const Position &position)
    : Statement(position)
    , mCondition(std::move(condition))
    , mBody(std::move(body))
{

}

void While::acceptVisitor(Visitor &visitor)
{
    visitor.visitWhile(*this);
}


IfThenElse::IfThenElse(std::unique_ptr<Expression> &&condition, std::unique_ptr<Scope> &&ifBlock, std::unique_ptr<Scope> &&elseBlock, const Position &position)
    : Statement(position)
    , mCondition(std::move(condition))
    , mIfBlock(std::move(ifBlock))
    , mElseBlock(std::move(elseBlock))
{

}

void IfThenElse::acceptVisitor(Visitor &visitor)
{
    visitor.visitIfThenElse(*this);
}

IfThen::IfThen(std::unique_ptr<Expression> &&condition, std::unique_ptr<Scope> &&ifBlock, const Position &position)
    : Statement(position)
    , mCondition(std::move(condition))
    , mIfBlock(std::move(ifBlock))
{

}

void IfThen::acceptVisitor(Visitor &visitor)
{
    visitor.visitIfThen(*this);
}

FloatLiteral::FloatLiteral(double value, const Position &position)
    : Singular(position)
    , mValue(value)
{}

void FloatLiteral::acceptVisitor(Visitor &visitor)
{
    visitor.visitFloatLiteral(*this);
}

Node::Node(const Position &position)
    : mPosition(position) {}

BooleanLiteral::BooleanLiteral(bool value, const Position &position)
    : Singular(position)
    , mValue(value)
{

}

void BooleanLiteral::acceptVisitor(Visitor &visitor)
{
    visitor.visitBooleanLiteral(*this);
}

StringLiteral::StringLiteral(const std::string &value, const Position &position)
    : Singular(position)
    , mValue(value)
{

}

void StringLiteral::acceptVisitor(Visitor &visitor)
{
    visitor.visitStringLiteral(*this);
}

Or::Or(std::unique_ptr<Expression> &&left, std::unique_ptr<Expression> &&right, const Position &position)
    : Expression(position)
    , mLeft(std::move(left))
    , mRight(std::move(right))
{

}

void Or::acceptVisitor(Visitor &visitor)
{
    visitor.visitOr(*this);
}

And::And(std::unique_ptr<Expression> &&left, std::unique_ptr<Expression> &&right, const Position &position)
    : Expression(position)
    , mLeft(std::move(left))
    , mRight(std::move(right))
{

}

void And::acceptVisitor(Visitor &visitor)
{
    visitor.visitAnd(*this);
}

Comparison::Comparison(std::unique_ptr<Expression> &&left, Token::Type op, std::unique_ptr<Expression> &&right, const Position & position)
    : Expression(position)
    , mOperators({op})
{
    mOperands.push_back(std::move(left));
    mOperands.push_back(std::move(right));
}

void Comparison::acceptVisitor(Visitor &visitor)
{
    visitor.visitComparison(*this);
}

void Comparison::addTest(Token::Type operator_, std::unique_ptr<Expression> &&operand)
{
    mOperators.push_back(operator_);
    mOperands.push_back(std::move(operand));
}

void Free::acceptVisitor(Visitor &visitor)
{
    visitor.visitFree();
}

For::For(std::unique_ptr<Name> loopVariable, std::unique_ptr<Expression> range, std::unique_ptr<Scope> body, const Position &position)
    : Statement(position)
    , mLoopVariable(std::move(loopVariable))
    , mRange(std::move(range))
    , mBody(std::move(body))
{

}

void For::acceptVisitor(Visitor &visitor)
{

}



} // namespace ast
