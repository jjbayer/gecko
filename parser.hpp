#pragma once
#include "ast.hpp"
#include "tokenizer.hpp"




using TokenIterator = std::vector<Token>::const_iterator;

std::unique_ptr<ast::Scope> parseScope(TokenIterator & it, const TokenIterator & end, int indent);

std::unique_ptr<ast::Statement> parseStatement(TokenIterator & it, const TokenIterator & end, int indent);

std::unique_ptr<ast::Statement> parseAssignment(TokenIterator & it, const TokenIterator & end, int indent);

/// ATTN: this might return nullptr
std::unique_ptr<ast::Assignee> parseAssignee(TokenIterator & it, const TokenIterator & end, int indent);

std::unique_ptr<ast::Expression> parseExpression(TokenIterator & it, const TokenIterator & end, int indent);

// TODO: parse all types of operators
std::unique_ptr<ast::Expression> parseLessThan(TokenIterator & it, const TokenIterator & end, int indent);

std::unique_ptr<ast::Expression> parseSum(TokenIterator & it, const TokenIterator & end, int indent);

std::unique_ptr<ast::Expression> parseMultiplication(TokenIterator & it, const TokenIterator & end, int indent);

std::unique_ptr<ast::Expression> parseFactor(TokenIterator & it, const TokenIterator & end, int indent);

std::unique_ptr<ast::Singular> parseSingular(TokenIterator & it, const TokenIterator & end, int indent);

std::unique_ptr<ast::Singular> parseFunctionCall(TokenIterator & it, const TokenIterator & end, int indent);

std::unique_ptr<ast::While> parseWhile(TokenIterator & it, const TokenIterator & end, int indent);


// TODO: more operators: or, and, <, >, <=, ...
// TODO: multiline nested expressions


