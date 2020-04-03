#include "tokenizer.hpp"
#include "statemachine.hpp"
#include <map>
#include <memory>

std::vector<Token> Tokenizer::tokenize(const std::string &input)
{
    std::vector<Token> tokens(1);

    auto start = std::make_shared<StateInitial>();
    std::shared_ptr<State> currentState = start;

    Position position;
    for(auto it = input.begin(); it < input.end(); ) {
        currentState = currentState->handle(it, tokens, position);
    }

    if( tokens.rbegin()->type == Token::Undefined ) {
        tokens.pop_back();
    }

    // HACKish
    for(auto & token : tokens) {
        static const std::map<std::string, Token::Type> keywords {
            {"if", Token::If},
            {"else", Token::Else},
            {"while", Token::While},
            {"for", Token::For},
            {"switch", Token::Switch},
            {"enum", Token::Enum},
            {"struct", Token::Struct},
            {"true", Token::True},
            {"false", Token::False},
            {"and", Token::And},
            {"or", Token::Or},
            {"free", Token::Free},
            {"for", Token::For},
            {"in", Token::In},
            {"function", Token::Function},
        };

        auto it = keywords.find(token.value);
        if( it != keywords.end() ) {
            token.type = it->second;
        }
    }

    return tokens;
}


const std::string & tokenName(Token::Type tokenType) {
    static const std::map<Token::Type, std::string> names {
        {Token::And, "And"},
        {Token::Assign, "Assign"},
        {Token::BraceLeft, "BraceLeft"},
        {Token::BraceRight, "BraceRight"},
        {Token::BracketLeft, "BracketLeft"},
        {Token::BracketRight, "BracketRight"},
        {Token::Colon, "Colon"},
        {Token::Comma, "Comma"},
        {Token::DivideBy, "DivideBy"},
        {Token::Else, "Else"},
        {Token::Enum, "Enum"},
        {Token::Equal, "Equal"},
        {Token::False, "False"},
        {Token::FloatLiteral, "FloatLiteral"},
        {Token::For, "For"},
        {Token::Free, "Free"},
        {Token::Function, "Function"},
        {Token::GreaterThan, "GreaterThan"},
        {Token::GTE, "GTE"},
        {Token::If, "If"},
        {Token::In, "In"},
        {Token::Indent, "Indent"},
        {Token::IntLiteral, "IntLiteral"},
        {Token::LessThan, "LessThan"},
        {Token::LineBreak, "LineBreak"},
        {Token::LTE, "LTE"},
        {Token::Minus, "Minus"},
        {Token::Name, "Name"},
        {Token::NotEqual, "NotEqual"},
        {Token::Or, "Or"},
        {Token::ParenLeft, "ParenLeft"},
        {Token::ParenRight, "ParenRight"},
        {Token::Plus, "Plus"},
        {Token::StringLiteral, "StringLiteral"},
        {Token::Struct, "Struct"},
        {Token::Switch, "Switch"},
        {Token::Times, "Times"},
        {Token::True, "True"},
        {Token::TypeName, "TypeName"},
        {Token::Undefined, "Undefined"},
        {Token::While, "While"},
    };

    return names.at(tokenType);
}
