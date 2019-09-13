#include "tokenizer.hpp"

#define BOOST_TEST_MAIN
#if !defined( WIN32 )
    #define BOOST_TEST_DYN_LINK
#endif
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(test_tokenizer)
{
    const std::string program {"123.4 +567"};

    Tokenizer tokenizer;
    const auto tokens = tokenizer.tokenize(program);
    BOOST_REQUIRE_EQUAL(tokens.size(), 3);
    BOOST_CHECK_EQUAL(tokens[0].type, Token::FloatLiteral);
    BOOST_CHECK_EQUAL(tokens[0].value, "123.4");
    BOOST_CHECK_EQUAL(tokens[1].type, Token::Plus);
    BOOST_CHECK_EQUAL(tokens[2].type, Token::IntLiteral);
    BOOST_CHECK_EQUAL(tokens[2].value, "567");
}
