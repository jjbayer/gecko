#include "tokenizer.hpp"

#define BOOST_TEST_MAIN
#if !defined( WIN32 )
    #define BOOST_TEST_DYN_LINK
#endif
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(test_numeric_literals)
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

BOOST_AUTO_TEST_CASE(test_string_literals)
{
    const std::string program {"\"abc\"  \"A quick brown 'fox'. Did he jump?\" "};

    Tokenizer tokenizer;
    const auto tokens = tokenizer.tokenize(program);
    BOOST_REQUIRE_EQUAL(tokens.size(), 2);
    BOOST_CHECK_EQUAL(tokens[0].type, Token::StringLiteral);
    BOOST_CHECK_EQUAL(tokens[0].value, "abc");
    BOOST_CHECK_EQUAL(tokens[1].type, Token::StringLiteral);
    BOOST_CHECK_EQUAL(tokens[1].value, "A quick brown 'fox'. Did he jump?");
}

BOOST_AUTO_TEST_CASE(test_full_program)
{
    const std::string program {
        "x = 0\n"
        "while x < 10\n"
        "    x = x + 1\n"
        "print(x)"
    };

    Tokenizer tokenizer;
    const auto tokens = tokenizer.tokenize(program);

    BOOST_REQUIRE_EQUAL(tokens.size(), 20);
    BOOST_CHECK_EQUAL(tokens[0].type, Token::Name);
    BOOST_CHECK_EQUAL(tokens[0].value, "x");
    BOOST_CHECK_EQUAL(tokens[1].type, Token::Assign);
    BOOST_CHECK_EQUAL(tokens[2].type, Token::IntLiteral);
    BOOST_CHECK_EQUAL(tokens[2].value, "0");
    BOOST_CHECK_EQUAL(tokens[3].type, Token::LineBreak);
    BOOST_CHECK_EQUAL(tokens[4].type, Token::Name);
    BOOST_CHECK_EQUAL(tokens[4].value, "while");
    BOOST_CHECK_EQUAL(tokens[5].type, Token::Name);
    BOOST_CHECK_EQUAL(tokens[5].value, "x");
    BOOST_CHECK_EQUAL(tokens[6].type, Token::LessThan);
    BOOST_CHECK_EQUAL(tokens[7].type, Token::IntLiteral);
    BOOST_CHECK_EQUAL(tokens[7].value, "10");
    BOOST_CHECK_EQUAL(tokens[8].type, Token::LineBreak);
    BOOST_CHECK_EQUAL(tokens[9].type, Token::Indent);

    BOOST_CHECK_EQUAL(tokens[10].type, Token::Name);
    BOOST_CHECK_EQUAL(tokens[10].value, "x");
    BOOST_CHECK_EQUAL(tokens[11].type, Token::Assign);
    BOOST_CHECK_EQUAL(tokens[12].type, Token::Name);
    BOOST_CHECK_EQUAL(tokens[12].value, "x");
    BOOST_CHECK_EQUAL(tokens[13].type, Token::Plus);
    BOOST_CHECK_EQUAL(tokens[14].type, Token::IntLiteral);
    BOOST_CHECK_EQUAL(tokens[14].value, "1");
    BOOST_CHECK_EQUAL(tokens[15].type, Token::LineBreak);
    BOOST_CHECK_EQUAL(tokens[16].type, Token::Name);
    BOOST_CHECK_EQUAL(tokens[16].value, "print");
    BOOST_CHECK_EQUAL(tokens[17].type, Token::ParenLeft);
    BOOST_CHECK_EQUAL(tokens[18].type, Token::Name);
    BOOST_CHECK_EQUAL(tokens[18].value, "x");
    BOOST_CHECK_EQUAL(tokens[19].type, Token::ParenRight);









}
