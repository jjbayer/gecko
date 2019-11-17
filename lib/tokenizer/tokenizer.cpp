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
        };

        auto it = keywords.find(token.value);
        if( it != keywords.end() ) {
            token.type = it->second;
        }
    }

    return tokens;
}
