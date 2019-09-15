#include "parser.hpp"
#include <sstream>


std::unique_ptr<ast::Scope> parseScope(TokenIterator &it, const TokenIterator &end, int indent)
{
    auto scope = std::make_unique<ast::Scope>();

    while( it != end ) {

        if( it-> type == Token::LineBreak ) {
            it++;
            continue; // TODO: remove extra line breaks in tokenizer
        }

        const auto startOfLine = it;

        auto indentCounter = 0;
        while( it->type == Token::Indent ) {
            indentCounter++;
            it++;
        }

        if ( indentCounter == (indent - 1)) {
            // Back to original scope
            it = startOfLine;
            break;
        }

        if( indentCounter != indent ) {
            // TODO: Error hierarchy. IndentError is_a ParseError is_a ...
            // TODO: keep line and column no. with every token and AST node
            throw std::runtime_error("Unexpected indent " + std::to_string(indentCounter));
        }

        scope->addStatement( parseStatement(it, end, indent) );
    }

    return std::move(scope);
}

std::unique_ptr<ast::Statement> parseStatement(TokenIterator &it, const TokenIterator &end, int indent)
{
    // TODO: other statements. return, for, while...
    if( it == end ) throw std::runtime_error("Expected statement, got EOF");

    if( it->type == Token::While ) {
        it++; // Consume keyword

        return parseWhile(it, end, indent);
    }

    auto statement = parseAssignment(it, end, indent);

    if( it == end ) throw std::runtime_error("Expected line break, got EOF");

    // TODO: token type names
    if( it->type != Token::LineBreak ) throw std::runtime_error("Expected line break, got token '" + it->value + "'");

    it++; // consume line break

    return std::move(statement);
}

std::unique_ptr<ast::Statement> parseAssignment(TokenIterator &it, const TokenIterator &end, int indent)
{
    auto assignee = parseAssignee(it, end, indent);
    if( ! assignee ) {

        return parseExpression(it, end, indent);
    }

    if( it == end || it->type != Token::Assign ) {

        return assignee;
    }

    it++; // consume assignment operator

    auto value = parseExpression(it, end, indent);

    return std::make_unique<ast::Assignment>(std::move(assignee), std::move(value));
}

std::unique_ptr<ast::Assignee> parseAssignee(TokenIterator &it, const TokenIterator &end, int indent)
{
    // TODO: unit test for every exception
    if( it == end ) throw std::runtime_error("Expected assignee, got EOF");

    // TODO: not only names can be assigned to
    if( it->type != Token::Name ) throw std::runtime_error("Expected name, got token '" + it->value + "'");

    auto name = std::make_unique<ast::Name>(it->value);

    it++;

    if( it == end || it->type == Token::Assign ) {

        return std::move(name);
    }

    it--; // Give it another try

    return nullptr;
}

std::unique_ptr<ast::Expression> parseExpression(TokenIterator &it, const TokenIterator &end, int indent)
{
    return parseLessThan(it, end, indent);
}

std::unique_ptr<ast::Expression> parseLessThan(TokenIterator &it, const TokenIterator &end, int indent)
{
    auto lhs = parseSum(it, end, indent);
    if( it == end || it->type != Token::LessThan ) {

        return lhs;
    }

    it++; // Consume operator

    auto rhs = parseLessThan(it, end, indent);

    return std::make_unique<ast::LessThan>(std::move(lhs), std::move(rhs));
}

std::unique_ptr<ast::Expression> parseSum(TokenIterator &it, const TokenIterator &end, int indent)
{
    auto lhs = parseMultiplication(it, end, indent);
    if( it == end || it->type != Token::Plus ) {

        return lhs;
    }

    it++; // Consume operator

    auto rhs = parseSum(it, end, indent);

    return std::make_unique<ast::Addition>(std::move(lhs), std::move(rhs));
}

std::unique_ptr<ast::Expression> parseMultiplication(TokenIterator &it, const TokenIterator &end, int indent)
{
    auto lhs = parseFactor(it, end, indent);
    if( it == end || it->type != Token::Times ) {

        return lhs;
    }

    it++; // Consume operator

    auto rhs = parseMultiplication(it, end, indent);

    return std::make_unique<ast::Addition>(std::move(lhs), std::move(rhs));
}

std::unique_ptr<ast::Expression> parseFactor(TokenIterator &it, const TokenIterator &end, int indent)
{
    if( it == end ) throw std::runtime_error("Expected factor, got EOF");

    // TODO: unit test parenthesized expression
    if( it->type == Token::ParenLeft ) {
        it++; // consume opening parenthesis

        auto expr = parseExpression(it, end, indent);

        if( it == end ) throw std::runtime_error("Expected token ')', got EOF");
        if( it->type != Token::ParenRight ) throw std::runtime_error("Expected token ')', got token '" + it->value + "'");

        it++; // consume closing parenthesis

        return std::move(expr);
    }

    return parseSingular(it, end, indent);
}

std::unique_ptr<ast::Singular> parseSingular(TokenIterator &it, const TokenIterator &end, int indent)
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

    return parseFunctionCall(it, end, indent);
}

std::unique_ptr<ast::Singular> parseFunctionCall(TokenIterator &it, const TokenIterator &end, int indent)
{
    if( it == end ) throw std::runtime_error("Expected name, got EOF");

    // TODO: token type to string
    if( it->type != Token::Name ) throw std::runtime_error("Expected name, got token '" + it->value + "'");

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
       functionCall->addArgument(parseExpression(it, end, indent));
    } while( it != end && it->type == Token::Comma);

    if( it == end ) throw std::runtime_error("Expected token ')', got EOF");
    if( it->type != Token::ParenRight ) throw std::runtime_error("Expected token ')', got token '" + it->value + "'");

    it++; // consume closing parenthesis

    return std::move(functionCall);
}

std::unique_ptr<ast::While> parseWhile(TokenIterator &it, const TokenIterator &end, int indent)
{
    auto condition = parseExpression(it, end, indent);
    if( it == end ) throw std::runtime_error("Expected linebreak, got EOF");
    if( it->type != Token::LineBreak ) throw std::runtime_error("Expected linebreak, got token " + it->value);

    it++; // Consume newline
    auto body = parseScope(it, end, indent + 1);

    if( body->mStatements.empty() ) throw std::runtime_error("While statement with empty body");

    return std::make_unique<ast::While>(std::move(condition), std::move(body));
}


