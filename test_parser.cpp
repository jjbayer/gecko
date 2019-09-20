#include "parser.hpp"

#define BOOST_TEST_MAIN
#if !defined( WIN32 )
    #define BOOST_TEST_DYN_LINK
#endif
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(test_function_without_args)
{
    std::vector<Token> tokens {
        {Token::Name, "print"},
        {Token::ParenLeft, "("},
        {Token::ParenRight, ")"},
        {Token::LineBreak},
    };

    auto begin = tokens.cbegin();
    const auto end = tokens.cend();
    BOOST_CHECK_NO_THROW(parseScope(begin, end, 0));
}

