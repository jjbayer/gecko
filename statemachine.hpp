#pragma once
#include "tokenizer.hpp"
#include <memory>
#include <string>

class State
{
public:
    using Iterator = std::string::const_iterator;

    virtual const char * name() const = 0;
    virtual std::shared_ptr<State> handle(Iterator & it, std::vector<Token> & tokens) = 0;
};


class StateIntLiteral: public State
{
    const char * name() const override { return "int_literal"; }
    std::shared_ptr<State> handle(Iterator & it, std::vector<Token> & tokens) override;
};

class StateInitial: public State
{
public:
    const char * name() const override { return "initial"; }
    std::shared_ptr<State> handle(Iterator & it, std::vector<Token> & tokens) override;
};
