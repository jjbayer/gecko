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

    ct::Compiler compiler;
    ast->acceptVisitor(compiler);

    std::stringstream stream;
    getOutput().stdout = &stream;
    run(compiler.instructions(), compiler.numObjectIdsUsed());

    return stream.str();
}


// TODO: why not part of "make test"?
BOOST_AUTO_TEST_CASE(test_fibonacci)
{
    const auto code = R"###(
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
)###";

    BOOST_CHECK_EQUAL(eval(code), "13\n");
}

BOOST_AUTO_TEST_CASE(test_hello_world)
{
    BOOST_CHECK_EQUAL(eval("print(\"Hello, world!\")\n"), "Hello, world!\n");
}


BOOST_AUTO_TEST_CASE(void_function)
{
    const auto code = R"###(
function greet(name: String)
    print("Hello, " + name + "!")

greet("Joris")
)###";

    BOOST_CHECK_EQUAL(eval(code), "Hello, Joris!\n");
}


BOOST_AUTO_TEST_CASE(recursive_function)
{
    // Recursive functions are not supported.
    const auto code = R"###(
function foo()
    foo()
)###";
    BOOST_CHECK_THROW(eval(code), UnknownFunction);
}

BOOST_AUTO_TEST_CASE(function_exist)
{
    // Recursive functions are not supported.
    const auto code = R"###(
function foo()
    1

function foo()
    2
)###";
    BOOST_CHECK_THROW(eval(code), FunctionExists);
}

BOOST_AUTO_TEST_CASE(garbage_collection_barrier)
{
    // 'free' in user function should not be allowed to free objects
    // just because they are invisible to the function

    const auto code = R"###(
function my_free()
    free

x = "This is a string which should stay intact"
my_free()
print(x)
)###";

    BOOST_CHECK_EQUAL(eval(code), "This is a string which should stay intact\n");

}
