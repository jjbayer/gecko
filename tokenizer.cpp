#include "tokenizer.hpp"
#include "statemachine.hpp"
#include <memory>
#include <iostream>

std::vector<Token> Tokenizer::tokenize(const std::string &input)
{
    std::vector<Token> tokens(1);

    auto start = std::make_shared<StateInitial>();
    std::shared_ptr<State> currentState = start;
    for(auto it = input.begin(); it < input.end(); ) {
        std::cout << "Current state = " << currentState->name() << "\n";
        std::cout << "Current char = '" << *it << "'\n";
        currentState = currentState->handle(it, tokens);
        std::cout << "Next state = " << currentState->name() << "\n";
    }

    for(const auto & token : tokens) {
        std::cout << "Token of type '" << token.type << "' with value '"
                  << token.value << "'\n";
    }

    return tokens;
}
