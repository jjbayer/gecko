#pragma once
#include "tokenizer.hpp"
#include <memory>
#include <string>

class State
{
public:
    using Iterator = std::string::const_iterator;

    virtual const char * name() const = 0;
    virtual std::shared_ptr<State> handle(Iterator & it, std::vector<Token> & tokens, Position & position) = 0;

protected:
    /// Advance one character
    /// TODO: tokenize list of lines => position becomes implicit
    void advance(Iterator & it, Position & position);
};


class StateInitial: public State
{
public:
    const char * name() const override { return "initial"; }
    std::shared_ptr<State> handle(Iterator & it, std::vector<Token> & tokens, Position & position) override;
};


class StateNumericLiteral: public State
{
    const char * name() const override { return "numeric_literal"; }
    std::shared_ptr<State> handle(Iterator & it, std::vector<Token> & tokens, Position & position) override;
};


class StateStringLiteral: public State
{
    const char * name() const override { return "string_literal"; }
    std::shared_ptr<State> handle(Iterator & it, std::vector<Token> & tokens, Position & position) override;
};

class StateName: public State
{
    const char * name() const override { return "name"; }
    std::shared_ptr<State> handle(Iterator & it, std::vector<Token> & tokens, Position & position) override;
};

class StateIndent: public State
{
    const char * name() const override { return "ident"; }
    std::shared_ptr<State> handle(Iterator & it, std::vector<Token> & tokens, Position & position) override;
};

class StateComment: public State
{
    const char * name() const override { return "comment"; }
    std::shared_ptr<State> handle(Iterator & it, std::vector<Token> & tokens, Position & position) override;
};


