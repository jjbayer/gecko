#include "statemachine.hpp"
#include  "exceptions.hpp"



std::shared_ptr<State> StateInitial::handle(Iterator &it, std::vector<Token> & tokens, Position &position)
{
    if( tokens.rbegin()->type != Token::Undefined) {
        tokens.push_back({Token::Undefined, "", position}); // Always start with a fresh token
    }

    const auto c = *it;

    if( c == ' ' ) {
        advance(it, position);
        return std::make_shared<StateInitial>();
    }

    if( c == ',' ) {
        *tokens.rbegin() = {Token::Comma, &c};
        advance(it, position);
        return std::make_shared<StateInitial>();
    }

    if( c == '+' ) {
        *tokens.rbegin() = {Token::Plus, &c}; // TODO: unary operator
        advance(it, position);
        return std::make_shared<StateInitial>();
    }

    if( c == '-' ) {
        *tokens.rbegin() = {Token::Minus, &c}; // TODO: unary operator
        advance(it, position);
        return std::make_shared<StateInitial>();
    }

    if( c == '*' ) {
        *tokens.rbegin() = {Token::Times, &c};
        advance(it, position);
        return std::make_shared<StateInitial>();
    }

    if( c == '/' ) {
        *tokens.rbegin() = {Token::DivideBy, &c};
        advance(it, position);
        return std::make_shared<StateInitial>();
    }

    if( c == '=' ) {
        // TODO: parse '=='
        *tokens.rbegin() = {Token::Assign, &c};
        advance(it, position);
        if( )

        return std::make_shared<StateInitial>();
    }

    if( c == '<' ) {
        *tokens.rbegin() = {Token::LessThan, &c};
        advance(it, position);
        return std::make_shared<StateInitial>();
    }
    if( c == '>' ) {
        *tokens.rbegin() = {Token::GreaterThan, &c};
        advance(it, position);
        return std::make_shared<StateInitial>();
    }
    if( c == '(' ) {
        *tokens.rbegin() = {Token::ParenLeft, &c};
        advance(it, position);
        return std::make_shared<StateInitial>();
    }
    if( c == ')' ) {
        *tokens.rbegin() = {Token::ParenRight, &c};
        advance(it, position);
        return std::make_shared<StateInitial>();
    }
    if( c == '[' ) {
        *tokens.rbegin() = {Token::BracketLeft, &c};
        advance(it, position);
        return std::make_shared<StateInitial>();
    }
    if( c == ']' ) {
        *tokens.rbegin() = {Token::BraceRight, &c};
        advance(it, position);
        return std::make_shared<StateInitial>();
    }
    if( c == '{' ) {
        *tokens.rbegin() = {Token::BraceLeft, &c};
        advance(it, position);
        return std::make_shared<StateInitial>();
    }
    if( c == '}' ) {
        *tokens.rbegin() = {Token::BraceRight, &c};
        advance(it, position);
        return std::make_shared<StateInitial>();
    }

    if( c == '\n' ) {
        *tokens.rbegin() = {Token::LineBreak, "<linebreak>"};
        tokens.emplace_back(); // S.t. state indent starts with fresh
        advance(it, position);
        tokens.rbegin()->position = position;
        return std::make_shared<StateIndent>();
    }

    if( c >= '0' && c <= '9') {
        tokens.rbegin()->type = Token::IntLiteral;
        return std::make_shared<StateNumericLiteral>();
    }

    if( c == '_' || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ) {
        tokens.rbegin()->type = Token::Name;
        tokens.rbegin()->value += c; // s.t. StateName can include numbers
        advance(it, position);

        return std::make_shared<StateName>();
    }

    if ( c == '"' ) {
        tokens.rbegin()->type = Token::StringLiteral;
        advance(it, position);

        return std::make_shared<StateStringLiteral>(); // FIXME: what if string literal not closed?
    }

    throw UnexpectedCharacter(position, c);
}

std::shared_ptr<State> StateNumericLiteral::handle(Iterator &it, std::vector<Token> & tokens, Position &position)
{
    const auto c = *it;

    if( c == '.' ) {
        tokens.rbegin()->type = Token::FloatLiteral;
        tokens.rbegin()->value += c;
        advance(it, position);

        return std::make_shared<StateNumericLiteral>();
    }

    if( c >= '0' && c <= '9' ) {

        tokens.rbegin()->value += c;
        advance(it, position);

        return std::make_shared<StateNumericLiteral>();
    }

    return std::make_shared<StateInitial>();
}

std::shared_ptr<State> StateStringLiteral::handle(State::Iterator &it, std::vector<Token> &tokens, Position &position)
{
    const auto c = *it;

    if( c == '"') {
        advance(it, position);

        return std::make_shared<StateInitial>();
    }

    tokens.rbegin()->value += c;
    advance(it, position);

    return std::make_shared<StateStringLiteral>();
}

std::shared_ptr<State> StateName::handle(State::Iterator &it, std::vector<Token> &tokens, Position &position)
{
    const auto c = *it;

    if( (c >= '0' && c <= '9') || c == '_' || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ) {

        tokens.rbegin()->value += c;
        advance(it, position);

        return std::make_shared<StateName>();
    }

    return std::make_shared<StateInitial>();
}

std::shared_ptr<State> StateIndent::handle(State::Iterator &it, std::vector<Token> &tokens, Position &position)
{
    const auto c = *it;

    const auto & token = tokens.rbegin();
    if( token->type == Token::Indent && token->value == "    " ) {
        tokens.push_back({Token::Undefined, ""});
    }

    if( c == ' ' ) {
        tokens.rbegin()->type = Token::Indent;
        tokens.rbegin()->value += c;
        advance(it, position);

        return std::make_shared<StateIndent>();
    }

    return std::make_shared<StateInitial>();
}

void State::advance(State::Iterator &it, Position &position)
{
    const auto c = *it;
    it++;
    if( c == '\n') {
        position.lineNumber++;
        position.column = 1;
    } else {
        position.column++;
    }
}
