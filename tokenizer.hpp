#pragma once
#include <string>
#include <vector>


struct Token
{
    enum class Type
    {
        Unknown,
        LineBreak,
        Indent,
        Int,
        Float,
        String,
        Name,
        Plus,
        Minus,
        Times,
        DivideBy,
        ParenOpen,
        ParenClose,
        BracketOpen,
        BracketClose,
        BraceOpen,
        BraceClose
    };

    Type type;
};


class Tokenizer
{
public:
    std::vector<Token> tokenize(const std::string & input);
};
