#pragma once
#include "tokenizer/tokenizer.hpp"

#include <set>
#include <stdexcept>


class CompilerBug: public std::runtime_error
{
public:
    CompilerBug(const std::string & what)
        : std::runtime_error(what) {}
};


class MissingFeature: public CompilerBug
{
public:
    MissingFeature(const std::string & what)
        : CompilerBug(what) {}
};


class ProgammingError: public std::runtime_error
{
public:
    ProgammingError(const Position & position, const std::string & text)
        : std::runtime_error(
            "Line " + std::to_string(mPosition.lineNumber) + ", column " + std::to_string(position.column) + ": "
            + text)
        , mPosition(position)
    {}

    virtual const char * name() const = 0;

    const Position mPosition;
};


class SyntaxError: public ProgammingError
{
public:
    SyntaxError(const Position & position, const std::string & text)
        : ProgammingError(position, text)
    {}

};


class UnexpectedCharacter: public SyntaxError
{
public:
    UnexpectedCharacter(const Position & position, char c)
        : SyntaxError(position, std::string {"Unexpected character '"} +  c +  "'")
    {}

    const char * name() const override { return "UnexpectedCharacter"; }
};

class UnexpectedToken: public SyntaxError
{
public:
    UnexpectedToken(const Token & token, Token::Type expected)
        : SyntaxError(token.position, "Expected " +  tokenName(expected) + " , got '" + token.value + "'")
    {}

    UnexpectedToken(const Token & token, const std::set<Token::Type> & expected);

    const char * name() const override { return "UnexpectedToken"; }
};

class UnexpectedIndent: public SyntaxError
{
public:
    UnexpectedIndent(const Position & position, int indent, int expected)
        : SyntaxError(position, "Expected " +  std::to_string(expected) + " , got " + std::to_string(indent))
    {}

    const char * name() const override { return "UnexpectedIndent"; }
};

class UnexpectedEndOfFile: public SyntaxError
{
public:
    UnexpectedEndOfFile(Token::Type expected)
        : SyntaxError({}, "Expected " +  tokenName(expected))
    {}

    UnexpectedEndOfFile(const std::set<Token::Type> & expected);

    UnexpectedEndOfFile(const std::string & expected)
        : SyntaxError({}, "Expected " +  expected)
    {}

    const char * name() const override { return "UnexpectedEndOfFile"; }
};


class EmptyBody: public SyntaxError
{
public:
    EmptyBody(const Position & position, const std::string & bodyType)
        : SyntaxError(position, "Body of " + bodyType + " is empty")
    {}

    const char * name() const override { return "EmptyBody"; }
};


class CompileError: public ProgammingError
{
public:
    CompileError(const Position & position, const std::string & text)
        : ProgammingError(position, text)
    {}
};


class TypeMismatch: public CompileError
{
public:
    TypeMismatch(const Position & position, const std::string & text)
        : CompileError(position, text)
    {}

    const char * name() const override { return "TypeMismatch"; }
};


class UndefinedVariable: public CompileError
{
public:
    UndefinedVariable(const Position & position, const std::string & text)
        : CompileError(position, text)
    {}

    const char * name() const override { return "UndefinedVariable"; }
};


class UnknownFunction: public CompileError
{
public:
    UnknownFunction(const Position & position, const std::string & text)
        : CompileError(position, text)
    {}

    const char * name() const override { return "UnknownFunction"; }
};


class UnknownType: public CompileError
{
public:
    UnknownType(const Position & position, const std::string & text)
        : CompileError(position, text)
    {}

    const char * name() const override { return "UnknownType"; }
};


class FunctionExists: public CompileError
{
public:
    FunctionExists(const Position & position, const std::string & text)
        : CompileError(position, text)
    {}

    const char * name() const override { return "FunctionAlreadyExists"; }
};
