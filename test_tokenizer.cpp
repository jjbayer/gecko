#include "tokenizer.hpp"

#define BOOST_TEST_MAIN
#if !defined( WIN32 )
    #define BOOST_TEST_DYN_LINK
#endif
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(test_tokenizer)
{
    const std::string program {"123 +"};

    Tokenizer tokenizer;
    const auto tokens = tokenizer.tokenize(program);
//    BOOST_CHECK_EQUAL(tokens.size(), 3);
//    BOOST_CHECK_EQUAL(tokens[0].type, Token::Type::Float);
//    BOOST_CHECK_EQUAL(tokens[1].type, Token::Type::Plus);
//    BOOST_CHECK_EQUAL(tokens[2].type, Token::Type::Int);
}
