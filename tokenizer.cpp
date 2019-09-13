#include "tokenizer.hpp"
#include "statemachine.hpp"
#include <memory>

std::vector<Token> Tokenizer::tokenize(const std::string &input)
{
    std::vector<Token> tokens(1);

    auto start = std::make_shared<StateInitial>();
    std::shared_ptr<State> currentState = start;
    for(auto it = input.begin(); it < input.end(); ) {
        currentState = currentState->handle(it, tokens);
    }

    if( tokens.rbegin()->type == Token::Undefined ) {
        tokens.pop_back();
    }

    return tokens;
}
