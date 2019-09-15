#pragma once
#include <memory>
#include <vector>




namespace ast {


class Visitor;


class /* interface */ Node
{
public:
    virtual void acceptVisitor(Visitor &) = 0;
};


class Statement: public Node
{

};


class Expression: public Statement
{

};


class Singular: public Expression
{

};


class Assignee: public Singular
{

};


class Name: public Assignee
{
public:
    Name(const std::string && name);

    void acceptVisitor(Visitor & visitor) override;

    const std::string mName;
};


class IntLiteral: public Singular
{
public:
    IntLiteral(int value);

    void acceptVisitor(Visitor & visitor) override;

    const int64_t mValue;
};


class FunctionCall: public Singular
{
public:
    FunctionCall(std::unique_ptr<Name> && name);

    void addArgument(std::unique_ptr<Expression> && expression);

    void acceptVisitor(Visitor & visitor) override;

    std::unique_ptr<Name> mName;
    std::vector<std::unique_ptr<Expression> > mArguments;
};


class Addition: public Expression
{
public:
    Addition(std::unique_ptr<Singular> && left, std::unique_ptr<Expression> && right);

    void acceptVisitor(Visitor & visitor) override;

    std::unique_ptr<Singular> mLeft;
    std::unique_ptr<Expression> mRight;
};


class Assignment: public Statement
{
public:
    Assignment(std::unique_ptr<Assignee> && name, std::unique_ptr<Expression> && value);

    void acceptVisitor(Visitor & visitor) override;

    std::unique_ptr<Assignee> mAssignee;
    std::unique_ptr<Expression> mValue;
};


class Scope: public Node
{
public:
    void addStatement(std::unique_ptr<Statement> && statement);

    void acceptVisitor(Visitor & visitor) override;

    std::vector<std::unique_ptr<Statement> > mStatements;
};


class IfThenElse: public Statement
{
public:
    IfThenElse(std::unique_ptr<Expression> && condition,
             std::unique_ptr<Scope> && ifBlock,
             std::unique_ptr<Scope> && elseBlock);

    void acceptVisitor(Visitor & visitor) override;

    std::unique_ptr<Expression> mCondition;
    std::unique_ptr<Scope> mIfBlock;
    std::unique_ptr<Scope> mElseBlock;
};


class While: public Statement
{
public:
    While(std::unique_ptr<Expression> && condition, std::unique_ptr<Scope> && body);

    void acceptVisitor(Visitor & visitor) override;

    std::unique_ptr<Expression> mCondition;
    std::unique_ptr<Scope> mBody;
};


class LessThan: public Expression
{
public:
    LessThan(std::unique_ptr<Singular> && left, std::unique_ptr<Expression> && right);

    void acceptVisitor(Visitor & visitor) override;

    std::unique_ptr<Singular> mLeft;
    std::unique_ptr<Expression> mRight;
};

} // namespace ast
