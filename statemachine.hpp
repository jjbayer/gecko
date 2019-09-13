#pragma once
#include <memory>
#include <string>


class State
{
public:
    using Iterator = std::string::const_iterator;

    virtual const char * name() const = 0;
    virtual std::shared_ptr<State> handle(Iterator & it) = 0;
};


class StateIntLiteral: public State
{
    const char * name() const override { return "int_literal"; }
    std::shared_ptr<State> handle(Iterator & it) override;
};

class StateInitial: public State
{
public:
    const char * name() const override { return "initial"; }
    std::shared_ptr<State> handle(Iterator & it) override;
};
