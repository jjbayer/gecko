#pragma once
#include <string>
#include <vector>


struct Token
{
    enum Type {
        Undefined,
        IntLiteral,
        FloatLiteral,
        Plus
    };

    Type type = Undefined;
    std::string value = "";
};


class Tokenizer
{
public:
    std::vector<Token> tokenize(const std::string & input);
};
