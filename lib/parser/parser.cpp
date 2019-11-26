#include "parser.hpp"
#include "common/exceptions.hpp"

#include <unordered_set>
#include <sstream>


std::unique_ptr<ast::Scope> parseScope(TokenIterator &it, const TokenIterator &end, int indent)
{
    if( it == end) throw std::runtime_error("Empty scope.");

    auto scope = std::make_unique<ast::Scope>(it->position);

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

        if ( indentCounter < indent) {
            // Back to original scope
            it = startOfLine;
            break;
        }

        if( indentCounter != indent ) {
            throw UnexpectedIndent(it->position, indentCounter, indent);
        }

        scope->addStatement( parseStatement(it, end, indent) );
    }

    return scope;
}

std::unique_ptr<ast::Statement> parseStatement(TokenIterator &it, const TokenIterator &end, int indent)
{
    // TODO: other statements. return, for, ...
    if( it == end ) throw UnexpectedEndOfFile("statement");

    if( it->type == Token::While ) {

        return parseWhile(it, end, indent);
    }

    if( it->type == Token::For ) {

        return parseFor(it, end, indent);
    }

    if( it->type == Token::If ) {

        // TODO: parse if without else
        return parseIfThenElse(it, end, indent);
    }

    if( it->type == Token::Free ) {
        const auto position = it->position;
        it++;
        return std::make_unique<ast::Free>(position);
    }


    auto statement = parseAssignment(it, end, indent);

    if( it == end )  throw UnexpectedEndOfFile("line break");

    // TODO: token type names
    if( it->type != Token::LineBreak ) throw UnexpectedToken(*it, "line break");

    it++; // consume line break

    return statement;
}

std::unique_ptr<ast::Statement> parseAssignment(TokenIterator &it, const TokenIterator &end, int indent)
{
    auto assignee = parseAssignee(it, end, indent);
    if( ! assignee ) {

        return parseExpression(it, end, indent);
    }

    if( it == end || it->type != Token::Assign ) {

        return std::move(assignee);
    }

    it++; // consume assignment operator

    auto value = parseExpression(it, end, indent);

    const auto pos = assignee->position();
    return std::make_unique<ast::Assignment>(std::move(assignee),
                                             std::move(value),
                                             pos);
}

std::unique_ptr<ast::Assignee> parseAssignee(TokenIterator &it, const TokenIterator &end, int indent)
{
    // TODO: unit test for every exception
    if( it == end ) throw UnexpectedEndOfFile("assignee");

    // TODO: not only names can be assigned to
    if( it->type != Token::Name ) {

        return nullptr;
    }

    auto name = std::make_unique<ast::Name>(it->value, it->position);

    it++;

    if( it == end || it->type == Token::Assign ) {

        return std::move(name);
    }

    it--; // Give it another try

    return nullptr;
}

std::unique_ptr<ast::Expression> parseExpression(TokenIterator &it, const TokenIterator &end, int indent)
{
    return parseOr(it, end, indent);
}

std::unique_ptr<ast::Expression> parseOr(TokenIterator &it, const TokenIterator &end, int indent)
{
    auto lhs = parseAnd(it, end, indent);
    if( it == end || it->type != Token::Or ) {

        return lhs;
    }

    it++; // Consume operator

    auto rhs = parseOr(it, end, indent);

    const auto pos = lhs->position();

    return std::make_unique<ast::Or>(std::move(lhs), std::move(rhs), pos);
}

std::unique_ptr<ast::Expression> parseAnd(TokenIterator &it, const TokenIterator &end, int indent)
{
    auto lhs = parseComparison(it, end, indent);
    if( it == end || it->type != Token::And ) {

        return lhs;
    }

    it++; // Consume operator

    auto rhs = parseAnd(it, end, indent);

    const auto pos = lhs->position();
    return std::make_unique<ast::And>(std::move(lhs), std::move(rhs), pos);
}

std::unique_ptr<ast::Expression> parseComparison(TokenIterator &it, const TokenIterator &end, int indent)
{
    const std::unordered_set<Token::Type> operatorTypes {
        Token::LessThan,
        Token::LTE,
        Token::Equal,
        Token::NotEqual,
        Token::GTE,
        Token::GreaterThan,
    };

    auto lhs = parseSum(it, end, indent);
    if( it == end || ! operatorTypes.count(it->type) ) {

        return lhs;
    }

    const auto operatorType = it->type;
    it++; // Consume operator

    auto rhs = parseSum(it, end, indent);

    auto comparison = std::make_unique<ast::Comparison>(
                std::move(lhs), operatorType, std::move(rhs), lhs->position());

    while( it != end && operatorTypes.count(it->type)) {

        const auto operatorType = it->type;
        it++; // Consume operator

        auto operand = parseSum(it, end, indent);

        comparison->addTest(operatorType, std::move(operand));
    }

    return std::move(comparison);
}

std::unique_ptr<ast::Expression> parseSum(TokenIterator &it, const TokenIterator &end, int indent)
{
    auto lhs = parseMultiplication(it, end, indent);
    if( it == end || it->type != Token::Plus ) {

        return lhs;
    }

    it++; // Consume operator

    auto rhs = parseSum(it, end, indent);

    const auto pos = lhs->position();
    return std::make_unique<ast::Addition>(std::move(lhs), std::move(rhs), pos);
}

std::unique_ptr<ast::Expression> parseMultiplication(TokenIterator &it, const TokenIterator &end, int indent)
{
    auto lhs = parseFactor(it, end, indent);
    if( it == end || it->type != Token::Times ) {

        return lhs;
    }

    it++; // Consume operator

    auto rhs = parseMultiplication(it, end, indent);

    // FIXME: return ast::Multiplication
    const auto pos = lhs->position();
    return std::make_unique<ast::Addition>(std::move(lhs), std::move(rhs), pos);
}

std::unique_ptr<ast::Expression> parseFactor(TokenIterator &it, const TokenIterator &end, int indent)
{
    if( it == end ) throw UnexpectedEndOfFile("factor");

    // TODO: unit test parenthesized expression
    if( it->type == Token::ParenLeft ) {
        it++; // consume opening parenthesis

        auto expr = parseExpression(it, end, indent);

        if( it == end ) throw UnexpectedEndOfFile(")");
        if( it->type != Token::ParenRight ) throw UnexpectedToken(*it, ")");

        it++; // consume closing parenthesis

        return expr;
    }

    return parseSingular(it, end, indent);
}

std::unique_ptr<ast::Singular> parseSingular(TokenIterator &it, const TokenIterator &end, int indent)
{
    if( it == end ) throw UnexpectedEndOfFile("singular expression");

    if( it->type == Token::IntLiteral ) {

        std::stringstream stream;
        stream << (it++)->value;
        int64_t value;
        stream >> value;

        return std::make_unique<ast::IntLiteral>(value, it->position);
    }

    if( it->type == Token::FloatLiteral ) {

        std::stringstream stream;
        stream << (it++)->value;
        double value;
        stream >> value;

        return std::make_unique<ast::FloatLiteral>(value, it->position);
    }

    if( it->type == Token::True ) {
        it++;
        return std::make_unique<ast::BooleanLiteral>(true, it->position);
    }

    if( it->type == Token::False ) {
        it++;
        return std::make_unique<ast::BooleanLiteral>(false, it->position);
    }

    if( it->type == Token::StringLiteral ) {
        auto literal = std::make_unique<ast::StringLiteral>(it->value, it->position);
        it++;
        return std::move(literal);
    }
    // TODO: float literal

    // TODO: dict literal

    // TODO: list literal

    return parseFunctionCall(it, end, indent);
}

std::unique_ptr<ast::Singular> parseFunctionCall(TokenIterator &it, const TokenIterator &end, int indent)
{
    if( it == end ) throw UnexpectedEndOfFile("name");
    // TODO: token type to string
    if( it->type != Token::Name ) throw UnexpectedToken(*it, "name");

    auto name = std::make_unique<ast::Name>(it->value, it->position);

    it++;

    if( it == end || it->type != Token::ParenLeft) {

        return std::move(name);
    }

    it++; // Consume opening parenthesis

    const auto pos = name->position();
    auto functionCall = std::make_unique<ast::FunctionCall>(std::move(name), pos);

    if( it == end ) throw UnexpectedEndOfFile("function arguments");

    if( it->type == Token::ParenRight) { // Empty argument list

        it++;

        return std::move(functionCall);
    }

    do {
       if( it->type == Token::Comma ) it++;
       functionCall->addArgument(parseExpression(it, end, indent));
    } while( it != end && it->type == Token::Comma);

    if( it == end ) throw UnexpectedEndOfFile("')");
    if( it->type != Token::ParenRight ) throw UnexpectedToken(*it, "')'");

    it++; // consume closing parenthesis

    return std::move(functionCall);
}

std::unique_ptr<ast::While> parseWhile(TokenIterator &it, const TokenIterator &end, int indent)
{
    const auto pos = it->position;
    it++; // consume 'while'

    auto condition = parseExpression(it, end, indent);
    if( it == end ) throw UnexpectedEndOfFile("line break");
    if( it->type != Token::LineBreak ) throw UnexpectedToken(*it, "line break");

    it++; // Consume newline
    auto body = parseScope(it, end, indent + 1);

    if( body->mStatements.empty() ) throw EmptyBody(it->position, "while");

    return std::make_unique<ast::While>(std::move(condition), std::move(body), pos);
}



std::unique_ptr<ast::Statement> parseIfThenElse(TokenIterator &it, const TokenIterator &end, int indent)
{
    const auto pos = it->position;
    it++; // consume 'if'

    auto condition = parseExpression(it, end, indent);
    if( it == end ) throw UnexpectedEndOfFile("line break");
    if( it->type != Token::LineBreak ) throw UnexpectedToken(*it, "line break");

    it++; // Consume newline
    auto ifBody = parseScope(it, end, indent + 1);

    if( ifBody->mStatements.empty() ) throw EmptyBody(it->position, "then");

    // TODO: single function to parse expected indent
    auto indentCounter = 0;
    const auto startOfLine = it;
    while( it != end && it->type == Token::Indent) {
        indentCounter++;
        it++;
    }

    const auto hasElse = (indentCounter == indent && it != end && it->type == Token::Else);

    if( ! hasElse ) {
        it = startOfLine;
        return std::make_unique<ast::IfThen>(std::move(condition), std::move(ifBody), pos);
    }

    // Else: else-block

    it++;

    if( it == end ) throw UnexpectedEndOfFile("line break");
    if( it->type != Token::LineBreak ) throw UnexpectedToken(*it, "line break");

    it++;

    auto elseBody = parseScope(it, end, indent + 1);

    return std::make_unique<ast::IfThenElse>(std::move(condition),
                                             std::move(ifBody),
                                             std::move(elseBody),
                                             pos);
}



std::unique_ptr<ast::For> parseFor(TokenIterator &it, const TokenIterator &end, int indent)
{
    const auto position = it->position;
    it++; // Consume "for"

    if( it == end ) throw UnexpectedEndOfFile("name");
    if( it->type != Token::Name ) {
        throw UnexpectedToken(*it, "name");
    }
    auto loopVar = std::make_unique<ast::Name>(it->value, it->position);

    it++; // Consume name

    if( it == end ) throw UnexpectedEndOfFile("in");
    if( it->type != Token::In ) {
        throw  UnexpectedToken(*it, "name");
    }

    it++; // Consume "in"

    if( it == end ) throw UnexpectedEndOfFile("expression");
    auto range = parseExpression(it, end, indent);

    if( it == end ) throw UnexpectedEndOfFile("line break");
    if( it->type != Token::LineBreak ) throw UnexpectedToken(*it, "line break");

    it++; // consume line break

    auto body = parseScope(it, end, indent + 1);

    return std::make_unique<ast::For>(std::move(loopVar), std::move(range), std::move(body), position);
}
