#include "ast.hpp"
#include  "compiler.hpp"
#include "executor.hpp"
#include "exceptions.hpp"
#include "printvisitor.hpp"

#include <iostream>

#define BOOST_TEST_MAIN
#if !defined( WIN32 )
    #define BOOST_TEST_DYN_LINK
#endif
#include <boost/test/unit_test.hpp>


BOOST_AUTO_TEST_CASE(test_compiler)
{
    using namespace ast;

    Scope program;

    program.addStatement(
        std::make_unique<Assignment>(
            std::make_unique<Name>("x"),
            std::make_unique<IntLiteral>(666)
        )
    );

    auto funCall = std::make_unique<FunctionCall>(
        std::make_unique<Name>("print")
    );
    funCall->addArgument(std::make_unique<Addition>(
                            std::make_unique<Name>("x"),
                            std::make_unique<IntLiteral>(123)
                        ));
    program.addStatement(std::move(funCall));

    program.addStatement(
        std::make_unique<Assignment>(
            std::make_unique<Name>("x"),
            std::make_unique<IntLiteral>(0)
        )
    );

    auto loopBody = std::make_unique<Scope>();
    {
        auto funCall = std::make_unique<FunctionCall>(
            std::make_unique<Name>("print")
        );
        funCall->addArgument(std::make_unique<Name>("x"));
        loopBody->addStatement(std::move(funCall));
    }
    loopBody->addStatement(
        std::make_unique<Assignment>(
            std::make_unique<Name>("x"),
            std::make_unique<Addition>(
                std::make_unique<Name>("x"),
                std::make_unique<IntLiteral>(1)
            )
        )
    );

    auto loop = std::make_unique<While>(
        std::make_unique<LessThan>(
            std::make_unique<Name>("x"),
            std::make_unique<IntLiteral>(10)
        ),
        std::move(loopBody)
    );
    program.addStatement(std::move(loop));

    {
        auto funCall = std::make_unique<FunctionCall>(
            std::make_unique<Name>("print")
        );
        funCall->addArgument(std::make_unique<Name>("x"));
        program.addStatement(std::move(funCall));
    }

    auto ifBody = std::make_unique<Scope>();
    {
        auto funCall = std::make_unique<FunctionCall>(
            std::make_unique<Name>("print")
        );
        funCall->addArgument(std::make_unique<IntLiteral>(100));
        ifBody->addStatement(std::move(funCall));
    }
    auto elseBody = std::make_unique<Scope>();
    {
        auto funCall = std::make_unique<FunctionCall>(
            std::make_unique<Name>("print")
        );
        funCall->addArgument(std::make_unique<IntLiteral>(200));
        elseBody->addStatement(std::move(funCall));
    }
    program.addStatement(std::make_unique<IfThenElse>(
        std::make_unique<IntLiteral>(0),
        std::move(ifBody),
        std::move(elseBody)
    ));

    std::cout << "BEGIN Output of executed program: \n";
    Compiler compiler;
    program.acceptVisitor(compiler);
    run(compiler.instructions());
    std::cout << "END\n";

}

BOOST_AUTO_TEST_CASE(test_undefined_variable)
{
    ast::Name x("x");
    Compiler compiler;
    BOOST_CHECK_THROW(x.acceptVisitor(compiler), UndefinedVariable);
}

BOOST_AUTO_TEST_CASE(test_out_of_scope)
{
    using namespace ast;

    auto loopBody = std::make_unique<Scope>();
    loopBody->addStatement(std::make_unique<Assignment>(
                               std::make_unique<Name>("x"),
                               std::make_unique<IntLiteral>(1))
                          );
    auto program = std::make_unique<Scope>();
    program->addStatement(std::make_unique<While>(
                           std::make_unique<IntLiteral>(1),
                           std::move(loopBody)));
    program->addStatement(std::make_unique<Name>("x"));

    Compiler compiler;
    BOOST_CHECK_THROW(program->acceptVisitor(compiler), UndefinedVariable);
}
