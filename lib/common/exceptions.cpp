#include "exceptions.hpp"


namespace {

    std::string tokenNames(const std::set<Token::Type> & types) {
        std::string ret;
        bool tail = false;
        for(auto type : types) {
            if( tail ) ret += ", ";
            ret += tokenName(type);
            tail = true;
        }

        return ret;
    }

}


UnexpectedToken::UnexpectedToken(const Token & token, const std::set<Token::Type> & expected)
    : SyntaxError(token.position, "Expected one of " +  tokenNames(expected) + " , got '" + token.value + "'")
{

}


UnexpectedEndOfFile::UnexpectedEndOfFile(const std::set<Token::Type> & expected)
    : SyntaxError({}, "Expected one of " + tokenNames(expected))
{

}
