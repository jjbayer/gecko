#include "parser.hpp"
#include <sstream>


std::unique_ptr<ast::Scope> parseScope(TokenIterator &it, const TokenIterator &end, int indent)
{
    auto scope = std::make_unique<ast::Scope>();

    while( it != end ) {

        auto indentCounter = 0;
        while( it->type == Token::Indent ) {
            indentCounter++;
            it++;
        }
        if( indentCounter != indent ) {
            // TODO: Error hierarchy. IndentError is_a ParseError is_a ...
            // TODO: keep line and column no. with every token and AST node
            throw std::runtime_error("Unexpected indent " + std::to_string(indentCounter));
        }

        scope->addStatement( parseStatement(it, end) );
    }
}

std::unique_ptr<ast::Statement> parseStatement(TokenIterator &it, const TokenIterator &end)
{
    // TODO: other statements. return, for, while...
    if( it == end ) throw std::runtime_error("Expected statement, got EOF");

    auto statement = parseAssignment(it, end);

    if( it == end ) throw std::runtime_error("Expected line break, got EOF");

    // TODO: token type names
    if( it->type != Token::LineBreak ) throw std::runtime_error("Expected line break, got token " + std::to_string(it->type));

    it++; // consume line break

    return std::move(statement);
}

std::unique_ptr<ast::Statement> parseAssignment(TokenIterator &it, const TokenIterator &end)
{
    auto assignee = parseAssignee(it, end);
    if( ! assignee ) {

        return parseExpression(it, end);
    }

    if( it == end || it->type != Token::Assign ) {

        return assignee;
    }

    it++; // consume assignment operator

    auto value = parseExpression(it, end);

    return std::make_unique<ast::Assignment>(std::move(assignee), std::move(value));
}

std::unique_ptr<ast::Assignee> parseAssignee(TokenIterator &it, const TokenIterator &end)
{
    // TODO: unit test for every exception
    if( it == end ) throw std::runtime_error("Expected assignee, got EOF");

    // TODO: not only names can be assigned to
    if( it->type != Token::Name ) throw std::runtime_error("Expected name, got token " + std::to_string(it->type));

    return std::make_unique<ast::Name>(it->value);
}

std::unique_ptr<ast::Expression> parseExpression(TokenIterator &it, const TokenIterator &end)
{
    return parseSum(it, end);
}

std::unique_ptr<ast::Expression> parseSum(TokenIterator &it, const TokenIterator &end)
{
    auto lhs = parseMultiplication(it, end);
    if( it == end || it->type != Token::Plus ) {

        return lhs;
    }

    auto rhs = parseSum(it, end);

    return std::make_unique<ast::Addition>(std::move(lhs), std::move(rhs));
}

std::unique_ptr<ast::Expression> parseMultiplication(TokenIterator &it, const TokenIterator &end)
{
    auto lhs = parseFactor(it, end);
    if( it == end || it->type != Token::Times ) {

        return lhs;
    }

    auto rhs = parseMultiplication(it, end);

    return std::make_unique<ast::Addition>(std::move(lhs), std::move(rhs));
}

std::unique_ptr<ast::Expression> parseFactor(TokenIterator &it, const TokenIterator &end)
{
    if( it == end ) throw std::runtime_error("Expected factor, got EOF");

    // TODO: unit test parenthesized expression
    if( it->type == Token::ParenLeft ) {
        it++; // consume opening parenthesis

        auto expr = parseExpression(it, end);

        if( it == end ) throw std::runtime_error("Expected token ')', got EOF");
        if( it->type != Token::ParenRight ) throw std::runtime_error("Expected token ')', got token " + std::to_string(it->type));

        it++; // consume closing parenthesis

        return std::move(expr);
    }

    return parseSingular(it, end);
}

std::unique_ptr<ast::Singular> parseSingular(TokenIterator &it, const TokenIterator &end)
{
    if( it == end ) throw std::runtime_error("Expected singular expression, got EOF");

    if( it->type == Token::IntLiteral ) {

        std::stringstream stream;
        stream << (it++)->value;
        int64_t value;
        stream >> value;

        return std::make_unique<ast::IntLiteral>(value);
    }
    // TODO: string literal
//    if( it->type == Token::StringLiteral ) return std::make_unique<ast::StringLiteral>((it++)->value);

    // TODO: float literal

    // TODO: dict literal

    // TODO: list literal

    return parseFunctionCall(it, end);
}

std::unique_ptr<ast::Singular> parseFunctionCall(TokenIterator &it, const TokenIterator &end)
{
    if( it == end ) throw std::runtime_error("Expected name, got EOF");

    // TODO: token type to string
    if( it->type != Token::Name ) throw std::runtime_error("Expected name, got token " + std::to_string(it->type));

    auto name = std::make_unique<ast::Name>(it->value);

    it++;

    if( it == end || it->type != Token::ParenLeft) {

        return std::move(name);
    }

    it++; // Consume opening parenthesis

    auto functionCall = std::make_unique<ast::FunctionCall>(std::move(name));

    if( it == end ) throw std::runtime_error("Expected function args, got EOF");

    if( it->type == Token::ParenRight) { // Empty argument list

        return std::move(functionCall);
    }

    do {
       if( it->type == Token::Comma ) it++;
       functionCall->addArgument(parseExpression(it, end));
    } while( it != end && it->type == Token::Comma);

    if( it == end ) throw std::runtime_error("Expected token ')', got EOF");
    if( it->type != Token::ParenRight ) throw std::runtime_error("Expected token ')', got token " + std::to_string(it->type));

    it++; // consume closing parenthesis

    return std::move(functionCall);
}
