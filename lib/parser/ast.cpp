#include "ast.hpp"
#include "visitor.hpp"
#include "common/utils.hpp"
#include "printvisitor.hpp"

#include <sstream>


namespace ast
{

Name::Name(const std::string &name, const Position &position)
    : Assignee(position)
    , mName(name)
{

}

void Name::acceptVisitor(Visitor &visitor) const
{
    visitor.visitName(*this);
}


TypeName::TypeName(const std::string &name, const Position &position)
    : Node(position)
    , mName(name)
{

}

void TypeName::acceptVisitor(Visitor &visitor) const
{
    visitor.visitTypeName(*this);
}


Type::Type(std::unique_ptr<TypeName> typeName, std::unique_ptr<TypeParameterList> typeParameters)
    : Node(typeName->position())
    , mTypeName(std::move(typeName))
    , mTypeParameters(std::move(typeParameters))
{

}

void Type::acceptVisitor(Visitor & visitor) const
{
    visitor.visitType(*this);
}

std::string Type::toString() const
{
    std::stringstream stream;
    auto visitor = PrintVisitor { stream };
    acceptVisitor(visitor);

    return stream.str();
}


TypeParameterList::TypeParameterList(const Position & position)
    : Node(position) {}

void TypeParameterList::acceptVisitor(Visitor & visitor) const
{
    visitor.visitTypeParameterList(*this);
}

void TypeParameterList::addTypeParameter(std::unique_ptr<Type> type)
{
    mTypeParameters.push_back(std::move(type));
}


IntLiteral::IntLiteral(int64_t value, const Position & position)
    : Singular(position)
    , mValue(value)
{

}

void IntLiteral::acceptVisitor(Visitor &visitor) const
{
    visitor.visitIntLiteral(*this);
}

Assignment::Assignment(std::unique_ptr<Assignee> &&name, std::unique_ptr<Expression> &&value)
    : Statement(name->position())
    , mAssignee(std::move(name))
    , mValue(std::move(value))
{
}

void Assignment::acceptVisitor(Visitor &visitor) const
{
    visitor.visitAssignment(*this);
}

void Scope::addStatement(std::unique_ptr<Statement> &&statement)
{
    mStatements.push_back(std::move(statement));
}

void Scope::acceptVisitor(Visitor &visitor) const
{
    visitor.visitScope(*this); // TODO: indent
}

FunctionCall::FunctionCall(std::unique_ptr<Name> name, std::unique_ptr<TypeParameterList> typeParameters)
    : Singular(name->position())
    , mName(std::move(name))
    , mTypeParameters(std::move(typeParameters))
{

}

void FunctionCall::addArgument(std::unique_ptr<Expression> &&expression)
{
    mArguments.push_back(std::move(expression));
}

void FunctionCall::acceptVisitor(Visitor &visitor) const
{
    visitor.visitFunctionCall(*this);
}

Addition::Addition(std::unique_ptr<Expression> &&left, std::unique_ptr<Expression> &&right)
    : Expression(left->position())
    , mLeft(std::move(left))
    , mRight(std::move(right))
{

}

void Addition::acceptVisitor(Visitor &visitor) const
{

    visitor.visitAddition(*this);
}

While::While(std::unique_ptr<Expression> &&condition, std::unique_ptr<Scope> &&body, const Position &position)
    : Statement(position)
    , mCondition(std::move(condition))
    , mBody(std::move(body))
{

}

void While::acceptVisitor(Visitor &visitor) const
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

void IfThenElse::acceptVisitor(Visitor &visitor) const
{
    visitor.visitIfThenElse(*this);
}

IfThen::IfThen(std::unique_ptr<Expression> &&condition, std::unique_ptr<Scope> &&ifBlock, const Position &position)
    : Statement(position)
    , mCondition(std::move(condition))
    , mIfBlock(std::move(ifBlock))
{

}

void IfThen::acceptVisitor(Visitor &visitor) const
{
    visitor.visitIfThen(*this);
}

FloatLiteral::FloatLiteral(double value, const Position &position)
    : Singular(position)
    , mValue(value)
{}

void FloatLiteral::acceptVisitor(Visitor &visitor) const
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

void BooleanLiteral::acceptVisitor(Visitor &visitor) const
{
    visitor.visitBooleanLiteral(*this);
}

StringLiteral::StringLiteral(const std::string &value, const Position &position)
    : Singular(position)
    , mValue(value)
{

}

void StringLiteral::acceptVisitor(Visitor &visitor) const
{
    visitor.visitStringLiteral(*this);
}

Or::Or(std::unique_ptr<Expression> &&left, std::unique_ptr<Expression> &&right)
    : Expression(left->position())
    , mLeft(std::move(left))
    , mRight(std::move(right))
{

}

void Or::acceptVisitor(Visitor &visitor) const
{
    visitor.visitOr(*this);
}

And::And(std::unique_ptr<Expression> &&left, std::unique_ptr<Expression> &&right)
    : Expression(left->position())
    , mLeft(std::move(left))
    , mRight(std::move(right))
{

}

void And::acceptVisitor(Visitor &visitor) const
{
    visitor.visitAnd(*this);
}

Comparison::Comparison(std::unique_ptr<Expression> &&left, Token::Type op, std::unique_ptr<Expression> &&right)
    : Expression(left->position())
    , mOperators({op})
{
    mOperands.push_back(std::move(left));
    mOperands.push_back(std::move(right));
}

void Comparison::acceptVisitor(Visitor &visitor) const
{
    visitor.visitComparison(*this);
}

void Comparison::addTest(Token::Type operator_, std::unique_ptr<Expression> &&operand)
{
    mOperators.push_back(operator_);
    mOperands.push_back(std::move(operand));
}

void Free::acceptVisitor(Visitor &visitor) const
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

void For::acceptVisitor(Visitor &visitor) const
{
    visitor.visitFor(*this);
}


FunctionDefinition::FunctionDefinition(std::unique_ptr<Name> functionName,
    std::vector<std::pair<std::unique_ptr<Name>, std::unique_ptr<Type> > > arguments,
    std::unique_ptr<Scope> body,
    const Position & position)
    : Statement(position)
    , mName(std::move(functionName))
    , mArguments(std::move(arguments))
    , mBody(std::move(body))
{

}

void FunctionDefinition::acceptVisitor(Visitor & visitor) const
{
    visitor.visitFunctionDefinition(*this);
}



} // namespace ast


std::size_t std::hash<ast::Type>::operator()(const ast::Type & type) const
{
    size_t seed {0};
    hash_combine(seed, type.mTypeName);

    if( type.mTypeParameters ) {
        for(const auto & type : type.mTypeParameters->mTypeParameters) {
            hash_combine(seed, *type);
        }
    }

    return seed;
}
