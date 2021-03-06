#pragma once
#include "ast.hpp"
#include "tokenizer/tokenizer.hpp"




using TokenIterator = std::vector<Token>::const_iterator;

std::unique_ptr<ast::Scope> parseScope(TokenIterator & it, const TokenIterator & end, int indent);

std::unique_ptr<ast::Statement> parseStatement(TokenIterator & it, const TokenIterator & end, int indent);

std::unique_ptr<ast::Statement> parseAssignment(TokenIterator & it, const TokenIterator & end, int indent);

/// ATTN: this might return nullptr
std::unique_ptr<ast::Assignee> parseAssignee(TokenIterator & it, const TokenIterator & end, int indent);

std::unique_ptr<ast::Expression> parseExpression(TokenIterator & it, const TokenIterator & end, int indent);

std::unique_ptr<ast::Expression> parseOr(TokenIterator & it, const TokenIterator & end, int indent);

std::unique_ptr<ast::Expression> parseAnd(TokenIterator & it, const TokenIterator & end, int indent);

std::unique_ptr<ast::Expression> parseComparison(TokenIterator & it, const TokenIterator & end, int indent);

std::unique_ptr<ast::Expression> parseSum(TokenIterator & it, const TokenIterator & end, int indent);

std::unique_ptr<ast::Expression> parseMultiplication(TokenIterator & it, const TokenIterator & end, int indent);

std::unique_ptr<ast::Expression> parseFactor(TokenIterator & it, const TokenIterator & end, int indent);

std::unique_ptr<ast::Singular> parseSingular(TokenIterator & it, const TokenIterator & end, int indent);

std::unique_ptr<ast::Type> parseType(TokenIterator & it, const TokenIterator & end, int indent);

std::unique_ptr<ast::TypeParameterList> parseTypeParameters(TokenIterator & it, const TokenIterator & end, int indent);

std::unique_ptr<ast::Singular> parseFunctionCall(TokenIterator & it, const TokenIterator & end, int indent);

std::unique_ptr<ast::While> parseWhile(TokenIterator & it, const TokenIterator & end, int indent);

std::unique_ptr<ast::Statement> parseIfThenElse(TokenIterator & it, const TokenIterator & end, int indent);

std::unique_ptr<ast::For> parseFor(TokenIterator & it, const TokenIterator & end, int indent);

std::unique_ptr<ast::FunctionDefinition> parseFunctionDefinition(TokenIterator & it, const TokenIterator & end, int indent);



// TODO: multiline nested expressions


