#include "statemachine.hpp"

std::shared_ptr<State> StateNumericLiteral::handle(Iterator &it, std::vector<Token> & tokens)
{
    const auto c = *it;

    if( c == '.' ) {
        tokens.rbegin()->type = Token::FloatLiteral;
        tokens.rbegin()->value += c;
        it++;

        return std::make_shared<StateNumericLiteral>();
    }

    if( c >= '0' && c <= '9' ) {

        tokens.rbegin()->value += c;
        it++;

        return std::make_shared<StateNumericLiteral>();
    }

    return std::make_shared<StateInitial>();
}

std::shared_ptr<State> StateInitial::handle(Iterator &it, std::vector<Token> & tokens)
{
    if( tokens.rbegin()->type != Token::Undefined) {
        tokens.emplace_back(); // Always start with a fresh token
    }

    const auto c = *it;

    if( c == ' ' ) {
        it++;
        return std::make_shared<StateInitial>();
    }

    if( c == '+' ) {
        *tokens.rbegin() = {Token::Plus}; // TODO: unary operator
        it++;
        return std::make_shared<StateInitial>();
    }

    if( c >= '0' && c <= '9') {
        tokens.rbegin()->type = Token::IntLiteral;
        return std::make_shared<StateNumericLiteral>();
    }

    throw std::runtime_error(std::string("Unexpected character '") + c + "'");
}
