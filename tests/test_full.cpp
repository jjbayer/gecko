#define BOOST_TEST_MAIN
#if !defined( WIN32 )
    #define BOOST_TEST_DYN_LINK
#endif
#include <boost/test/unit_test.hpp>


#include "common/exceptions.hpp"
#include "compiler/compiler.hpp"
#include "parser/ast.hpp"
#include "parser/parser.hpp"
#include "parser/printvisitor.hpp"
#include "runtime/executor.hpp"
#include "runtime/output.hpp"

#include <sstream>


std::string eval(const std::string & code)
{
    Tokenizer tokenizer;
    const auto tokens = tokenizer.tokenize(code);

    auto it = tokens.cbegin();
    const auto end = tokens.cend();
    const auto ast = parseScope(it, end, 0);

    static Compiler compiler;
    ast->acceptVisitor(compiler);

    std::stringstream stream;
    getOutput().stdout = &stream;
    run(compiler.instructions(), compiler.numObjectIdsUsed());

    return stream.str();
}


// TODO: why not part of "make test"?
BOOST_AUTO_TEST_CASE(test_fibonacci)
{
    const auto code = R"__code__(
f0 = 1
f1 = 1

i = 0
f = 0
while i < 5
    f = f0 + f1
    f0 = f1
    f1 = f

    i = i + 1
print(f)
)__code__";

    BOOST_CHECK_EQUAL(eval(code), "13\n");
}


