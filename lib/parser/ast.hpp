#pragma once
#include "tokenizer/tokenizer.hpp"  // For Posiion

#include <memory>
#include <vector>


class Position;


namespace ast {


class Visitor;


class /* interface */ Node
{
public:

    Node(const Position & position);

    virtual void acceptVisitor(Visitor &) = 0;

    const Position & position() const {  return mPosition; }

private:
    Position mPosition;
};


class Statement: public Node
{
public:
    Statement(const Position & position)
        :Node(position) {}
};


class Expression: public Statement
{
public:
    Expression(const Position & position)
        :Statement(position) {}
};


class Singular: public Expression
{
public:
    Singular(const Position & position)
        :Expression(position) {}
};


class Assignee: public Singular
{
public:
    Assignee(const Position & position)
        :Singular(position) {}
};


class Name: public Assignee
{
public:
    Name(const std::string & name, const Position & position);

    void acceptVisitor(Visitor & visitor) override;

    const std::string mName;
};


class TypeName: public Node
{
public:
    TypeName(const std::string & name, const Position & position);

    void acceptVisitor(Visitor & visitor) override;

    const std::string mName;
};


class IntLiteral: public Singular
{
public:
    IntLiteral(int64_t value, const Position & position);

    void acceptVisitor(Visitor & visitor) override;

    const int64_t mValue;
};


class FloatLiteral: public Singular
{
public:
    FloatLiteral(double value, const Position & position);

    void acceptVisitor(Visitor & visitor) override;

    const double mValue;
};


class BooleanLiteral: public Singular
{
public:
    BooleanLiteral(bool value, const Position & position);

    void acceptVisitor(Visitor & visitor) override;

    const bool mValue;
};


class StringLiteral: public Singular
{
public:
    StringLiteral(const std::string & value, const Position & position);

    void acceptVisitor(Visitor & visitor) override;

    const std::string mValue;
};


class FunctionCall: public Singular
{
public:
    FunctionCall(std::unique_ptr<Name> && name, const Position & position);

    void addArgument(std::unique_ptr<Expression> && expression);

    void acceptVisitor(Visitor & visitor) override;

    std::unique_ptr<Name> mName;
    std::vector<std::unique_ptr<Expression> > mArguments;
};


class Or: public Expression
{
public:
    Or(std::unique_ptr<Expression> && left, std::unique_ptr<Expression> && right, const Position & position);

    void acceptVisitor(Visitor & visitor) override;

    std::unique_ptr<Expression> mLeft;
    std::unique_ptr<Expression> mRight;
};


class And: public Expression
{
public:
    And(std::unique_ptr<Expression> && left, std::unique_ptr<Expression> && right, const Position & position);

    void acceptVisitor(Visitor & visitor) override;

    std::unique_ptr<Expression> mLeft;
    std::unique_ptr<Expression> mRight;
};


class Addition: public Expression
{
public:
    Addition(std::unique_ptr<Expression> && left, std::unique_ptr<Expression> && right, const Position & position);

    void acceptVisitor(Visitor & visitor) override;

    std::unique_ptr<Expression> mLeft;
    std::unique_ptr<Expression> mRight;
};


class Assignment: public Statement
{
public:
    Assignment(std::unique_ptr<Assignee> && name, std::unique_ptr<Expression> && value, const Position & position);

    void acceptVisitor(Visitor & visitor) override;

    std::unique_ptr<Assignee> mAssignee;
    std::unique_ptr<Expression> mValue;
};


class Scope: public Node
{
public:

    Scope(const Position & position): Node(position) {}

    void addStatement(std::unique_ptr<Statement> && statementn);

    void acceptVisitor(Visitor & visitor) override;

    std::vector<std::unique_ptr<Statement> > mStatements;
};


class IfThen: public Statement
{
public:
    IfThen(std::unique_ptr<Expression> && condition,
             std::unique_ptr<Scope> && ifBlock, const Position & position);

    void acceptVisitor(Visitor & visitor) override;

    std::unique_ptr<Expression> mCondition;
    std::unique_ptr<Scope> mIfBlock;
};


class IfThenElse: public Statement
{
public:
    IfThenElse(std::unique_ptr<Expression> && condition,
             std::unique_ptr<Scope> && ifBlock,
             std::unique_ptr<Scope> && elseBlock, const Position & position);

    void acceptVisitor(Visitor & visitor) override;

    std::unique_ptr<Expression> mCondition;
    std::unique_ptr<Scope> mIfBlock;
    std::unique_ptr<Scope> mElseBlock;
};


class While: public Statement
{
public:
    While(std::unique_ptr<Expression> && condition, std::unique_ptr<Scope> && body, const Position & position);

    void acceptVisitor(Visitor & visitor) override;

    std::unique_ptr<Expression> mCondition;
    std::unique_ptr<Scope> mBody;
};


class Comparison: public Expression
{
public:

    /// Comparison needs at least 2 operands, but more can be added later
    Comparison(std::unique_ptr<Expression> && left, Token::Type op, std::unique_ptr<Expression> && right, const Position &position);

    void acceptVisitor(Visitor & visitor) override;

    void addTest(Token::Type, std::unique_ptr<Expression> && operand);

    std::vector<std::unique_ptr<Expression> > mOperands;
    std::vector<Token::Type> mOperators;
};


class Free: public Statement
{
public:
    Free(const Position & position): Statement(position) {}

    void acceptVisitor(Visitor & visitor) override;
};


class For: public Statement
{
public:
    For(std::unique_ptr<Name> loopVariable, std::unique_ptr<Expression> range, std::unique_ptr<Scope> body, const Position & position);

    void acceptVisitor(Visitor & visitor) override;

    std::unique_ptr<Name> mLoopVariable;
    std::unique_ptr<Expression> mRange;
    std::unique_ptr<Scope> mBody;
};


class FunctionDefinition: public Statement
{
public:
    FunctionDefinition(
        std::unique_ptr<Name> functionName,
        std::vector<std::pair<std::unique_ptr<Name>, std::unique_ptr<TypeName> > > arguments,
        std::unique_ptr<Scope> body,
        const Position & position
    );

    void acceptVisitor(Visitor & visitor) override;

    std::unique_ptr<Name> mName;
    std::vector<std::pair<std::unique_ptr<Name>, std::unique_ptr<TypeName> > > mArguments;
    std::unique_ptr<Scope> mBody;
};


} // namespace ast
