#pragma once
#include <string>
#include <vector>


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

        ParenLeft,
        ParenRight,
        BracketLeft,
        BracketRight,
        BraceLeft,
        BraceRight,

        LineBreak,
        Indent,
    };

    Type type = Undefined;
    std::string value = "";
};


class Tokenizer
{
public:
    std::vector<Token> tokenize(const std::string & input);
};
