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
        tokens.push_back({Token::Undefined, ""}); // Always start with a fresh token
    }

    const auto c = *it;

    if( c == ' ' ) {
        it++;
        return std::make_shared<StateInitial>();
    }

    if( c == ',' ) {
        *tokens.rbegin() = {Token::Comma, &c};
        it++;
        return std::make_shared<StateInitial>();
    }

    if( c == '+' ) {
        *tokens.rbegin() = {Token::Plus, &c}; // TODO: unary operator
        it++;
        return std::make_shared<StateInitial>();
    }
    if( c == '=' ) {
        *tokens.rbegin() = {Token::Assign, &c};
        it++;
        return std::make_shared<StateInitial>();
    }

    if( c == '<' ) {
        *tokens.rbegin() = {Token::LessThan, &c};
        it++;
        return std::make_shared<StateInitial>();
    }
    if( c == '>' ) {
        *tokens.rbegin() = {Token::GreaterThan, &c};
        it++;
        return std::make_shared<StateInitial>();
    }
    if( c == '(' ) {
        *tokens.rbegin() = {Token::ParenLeft, &c};
        it++;
        return std::make_shared<StateInitial>();
    }
    if( c == ')' ) {
        *tokens.rbegin() = {Token::ParenRight, &c};
        it++;
        return std::make_shared<StateInitial>();
    }
    if( c == '[' ) {
        *tokens.rbegin() = {Token::BracketLeft, &c};
        it++;
        return std::make_shared<StateInitial>();
    }
    if( c == ']' ) {
        *tokens.rbegin() = {Token::BraceRight, &c};
        it++;
        return std::make_shared<StateInitial>();
    }
    if( c == '{' ) {
        *tokens.rbegin() = {Token::BraceLeft, &c};
        it++;
        return std::make_shared<StateInitial>();
    }
    if( c == '}' ) {
        *tokens.rbegin() = {Token::BraceRight, &c};
        it++;
        return std::make_shared<StateInitial>();
    }

    if( c == '\n' ) {
        *tokens.rbegin() = {Token::LineBreak, "<linebreak>"};
        tokens.emplace_back(); // S.t. state indent starts with fresh
        it++;
        return std::make_shared<StateIndent>();
    }

    if( c >= '0' && c <= '9') {
        tokens.rbegin()->type = Token::IntLiteral;
        return std::make_shared<StateNumericLiteral>();
    }

    if( c == '_' || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ) {
        tokens.rbegin()->type = Token::Name;
        tokens.rbegin()->value += c; // s.t. StateName can include numbers
        it++;

        return std::make_shared<StateName>();
    }

    if ( c == '"' ) {
        tokens.rbegin()->type = Token::StringLiteral;
        it++;

        return std::make_shared<StateStringLiteral>(); // FIXME: what if string literal not closed?
    }

    throw std::runtime_error(std::string("Unexpected character '") + c + "'");
}

std::shared_ptr<State> StateStringLiteral::handle(State::Iterator &it, std::vector<Token> &tokens)
{
    const auto c = *it;

    if( c == '"') {
        it++;

        return std::make_shared<StateInitial>();
    }

    tokens.rbegin()->value += c;
    it++;

    return std::make_shared<StateStringLiteral>();
}

std::shared_ptr<State> StateName::handle(State::Iterator &it, std::vector<Token> &tokens)
{
    const auto c = *it;

    if( (c >= '0' && c <= '9') || c == '_' || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ) {

        tokens.rbegin()->value += c;
        it++;

        return std::make_shared<StateName>();
    }

    return std::make_shared<StateInitial>();
}

std::shared_ptr<State> StateIndent::handle(State::Iterator &it, std::vector<Token> &tokens)
{
    const auto c = *it;

    const auto & token = tokens.rbegin();
    if( token->type == Token::Indent && token->value == "    " ) {
        tokens.push_back({Token::Undefined, ""});
    }

    if( c == ' ' ) {
        tokens.rbegin()->type = Token::Indent;
        tokens.rbegin()->value += c;
        it++;

        return std::make_shared<StateIndent>();
    }

    return std::make_shared<StateInitial>();
}
