#include "statemachine.hpp"
#include  "common/exceptions.hpp"



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
        *tokens.rbegin() = {Token::Comma, &c, position};
        advance(it, position);
        return std::make_shared<StateInitial>();
    }

    if( c == '+' ) {
        *tokens.rbegin() = {Token::Plus, &c, position}; // TODO: unary operator
        advance(it, position);
        return std::make_shared<StateInitial>();
    }

    if( c == '-' ) {
        *tokens.rbegin() = {Token::Minus, &c, position}; // TODO: unary operator
        advance(it, position);
        return std::make_shared<StateInitial>();
    }

    if( c == '*' ) {
        *tokens.rbegin() = {Token::Times, &c, position};
        advance(it, position);
        return std::make_shared<StateInitial>();
    }

    if( c == '/' ) {
        *tokens.rbegin() = {Token::DivideBy, &c, position};
        advance(it, position);
        return std::make_shared<StateInitial>();
    }

    if( c == '=' ) {
        // TODO: parse '=='
        *tokens.rbegin() = {Token::Assign, &c, position};
        advance(it, position);

        return std::make_shared<StateInitial>();
    }

    if( c == '<' ) {
        *tokens.rbegin() = {Token::LessThan, &c, position};
        advance(it, position);
        return std::make_shared<StateInitial>();
    }
    if( c == '>' ) {
        *tokens.rbegin() = {Token::GreaterThan, &c, position};
        advance(it, position);
        return std::make_shared<StateInitial>();
    }
    if( c == '(' ) {
        *tokens.rbegin() = {Token::ParenLeft, &c, position};
        advance(it, position);
        return std::make_shared<StateInitial>();
    }
    if( c == ')' ) {
        *tokens.rbegin() = {Token::ParenRight, &c, position};
        advance(it, position);
        return std::make_shared<StateInitial>();
    }
    if( c == '[' ) {
        *tokens.rbegin() = {Token::BracketLeft, &c, position};
        advance(it, position);
        return std::make_shared<StateInitial>();
    }
    if( c == ']' ) {
        *tokens.rbegin() = {Token::BraceRight, &c, position};
        advance(it, position);
        return std::make_shared<StateInitial>();
    }
    if( c == '{' ) {
        *tokens.rbegin() = {Token::BraceLeft, &c, position};
        advance(it, position);
        return std::make_shared<StateInitial>();
    }
    if( c == '}' ) {
        *tokens.rbegin() = {Token::BraceRight, &c, position};
        advance(it, position);
        return std::make_shared<StateInitial>();
    }
    if( c == ':' ) {
        *tokens.rbegin() = {Token::Colon, &c, position};
        advance(it, position);
        return std::make_shared<StateInitial>();
    }

    if( c == '\n' ) {
        *tokens.rbegin() = {Token::LineBreak, "<linebreak>", position};
        tokens.emplace_back(); // S.t. state indent starts with fresh
        advance(it, position);
        tokens.rbegin()->position = position;
        return std::make_shared<StateIndent>();
    }

    if( c >= '0' && c <= '9') {
        tokens.rbegin()->type = Token::IntLiteral;
        return std::make_shared<StateNumericLiteral>();
    }

    if( c >= 'A' && c <= 'Z' ) {
        tokens.rbegin()->type = Token::TypeName;
        tokens.rbegin()->value += c; // s.t. StateName can include numbers
        advance(it, position);

        return std::make_shared<StateName>();
    }

    if( c == '_' || (c >= 'a' && c <= 'z') ) {
        tokens.rbegin()->type = Token::Name;
        tokens.rbegin()->value += c; // s.t. StateName can include numbers
        advance(it, position);

        return std::make_shared<StateName>();
    }

    if ( c == '"' ) {
        advance(it, position);
        tokens.rbegin()->type = Token::StringLiteral;

        return std::make_shared<StateStringLiteral>(); // FIXME: what if string literal not closed?
    }

    if( c == '#' ) {
        advance(it, position);
        return std::make_shared<StateComment>();
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
        tokens.push_back({Token::Undefined, "", position});
    }

    if( c == ' ' ) {
        tokens.rbegin()->type = Token::Indent;
        tokens.rbegin()->value += c;
        advance(it, position);

        return std::make_shared<StateIndent>();
    }

    return std::make_shared<StateInitial>();
}

std::shared_ptr<State> StateComment::handle(State::Iterator &it, std::vector<Token> &tokens, Position &position)
{
    if( *it == '\n') {
        return std::make_shared<StateInitial>();
    } else {
        advance(it, position);
        return std::make_shared<StateComment>();
    }
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
