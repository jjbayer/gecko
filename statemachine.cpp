#include "statemachine.hpp"

std::shared_ptr<State> StateIntLiteral::handle(Iterator &it)
{
    const auto c = *it;

    if( c >= '0' && c <= '9') {
        it++;

        return std::make_shared<StateIntLiteral>();
    }

    return std::make_shared<StateInitial>();
}

std::shared_ptr<State> StateInitial::handle(Iterator &it)
{
    const auto c = *it;

    if( c >= '0' && c <= '9') {

        return std::make_shared<StateIntLiteral>();
    }

    throw std::runtime_error(std::string("Unexpected character '") + c + "'");
}
