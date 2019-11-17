#pragma once
#include <string>
#include <vector>

struct Position
{
    int lineNumber = 1;
    int column = 1;
};

struct Token
{
    enum Type {
        Undefined,

        IntLiteral,
        FloatLiteral,
        StringLiteral,
        Name,

        Assign,

        Plus,
        Minus,
        Times,
        DivideBy,

        Equal,
        NotEqual,
        LessThan,
        LTE,
        GreaterThan,
        GTE,

        // TODO: (bitwise) and / or
        // TODO: unary operators

        Comma,
        ParenLeft,
        ParenRight,
        BracketLeft,
        BracketRight,
        BraceLeft,
        BraceRight,

        LineBreak,
        Indent,

        If,
        Else,
        While,
        For,
        Switch,
        Enum,
        Struct,

        True,
        False,
        And,
        Or,

        Free,
    };

    Type type = Undefined;
    std::string value = "";

    Position position;
};


class Tokenizer
{
public:
    std::vector<Token> tokenize(const std::string & input);
};
