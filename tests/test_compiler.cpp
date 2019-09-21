#include "common/exceptions.hpp"
#include "compiler/compiler.hpp"
#include "parser/ast.hpp"
#include "parser/printvisitor.hpp"
#include "runtime/executor.hpp"

#include <iostream>

#define BOOST_TEST_MAIN
#if !defined( WIN32 )
    #define BOOST_TEST_DYN_LINK
#endif
#include <boost/test/unit_test.hpp>


const Position dummyPosition;


BOOST_AUTO_TEST_CASE(test_compiler)
{
    using namespace ast;

    Scope program(dummyPosition);

    program.addStatement(
        std::make_unique<Assignment>(
            std::make_unique<Name>("x", dummyPosition),
            std::make_unique<IntLiteral>(666, dummyPosition),
            dummyPosition
        )
    );

    auto funCall = std::make_unique<FunctionCall>(
        std::make_unique<Name>("print", dummyPosition), dummyPosition
    );
    funCall->addArgument(std::make_unique<Addition>(
                            std::make_unique<Name>("x", dummyPosition),
                            std::make_unique<IntLiteral>(123, dummyPosition),
                            dummyPosition
                        ));
    program.addStatement(std::move(funCall));

    program.addStatement(
        std::make_unique<Assignment>(
            std::make_unique<Name>("x", dummyPosition),
            std::make_unique<IntLiteral>(0, dummyPosition), dummyPosition
        )
    );

    auto loopBody = std::make_unique<Scope>(dummyPosition);
    {
        auto funCall = std::make_unique<FunctionCall>(
            std::make_unique<Name>("print", dummyPosition), dummyPosition
        );
        funCall->addArgument(std::make_unique<Name>("x", dummyPosition));
        loopBody->addStatement(std::move(funCall));
    }
    loopBody->addStatement(
        std::make_unique<Assignment>(
            std::make_unique<Name>("x", dummyPosition),
            std::make_unique<Addition>(
                std::make_unique<Name>("x", dummyPosition),
                std::make_unique<IntLiteral>(1, dummyPosition), dummyPosition
            ), dummyPosition
        )
    );

    auto loop = std::make_unique<While>(
        std::make_unique<LessThan>(
            std::make_unique<Name>("x", dummyPosition),
            std::make_unique<IntLiteral>(10, dummyPosition),
            dummyPosition
        ),
        std::move(loopBody), dummyPosition
    );
    program.addStatement(std::move(loop));

    {
        auto funCall = std::make_unique<FunctionCall>(
            std::make_unique<Name>("print", dummyPosition), dummyPosition
        );
        funCall->addArgument(std::make_unique<Name>("x", dummyPosition));
        program.addStatement(std::move(funCall));
    }

    auto ifBody = std::make_unique<Scope>(dummyPosition);
    {
        auto funCall = std::make_unique<FunctionCall>(
            std::make_unique<Name>("print", dummyPosition), dummyPosition
        );
        funCall->addArgument(std::make_unique<IntLiteral>(100, dummyPosition));
        ifBody->addStatement(std::move(funCall));
    }
    auto elseBody = std::make_unique<Scope>(dummyPosition);
    {
        auto funCall = std::make_unique<FunctionCall>(
            std::make_unique<Name>("print", dummyPosition), dummyPosition
        );
        funCall->addArgument(std::make_unique<IntLiteral>(200, dummyPosition));
        elseBody->addStatement(std::move(funCall));
    }
    program.addStatement(std::make_unique<IfThenElse>(
        std::make_unique<IntLiteral>(0, dummyPosition),
        std::move(ifBody),
        std::move(elseBody), dummyPosition
    ));

    std::cout << "BEGIN Output of executed program: \n";
    Compiler compiler;
    program.acceptVisitor(compiler);
    run(compiler.instructions(), compiler.numObjectIdsUsed());
    std::cout << "END\n";

}

BOOST_AUTO_TEST_CASE(test_undefined_variable)
{
    ast::Name x("x", dummyPosition);
    Compiler compiler;
    BOOST_CHECK_THROW(x.acceptVisitor(compiler), UndefinedVariable);
}

BOOST_AUTO_TEST_CASE(test_out_of_scope)
{
    using namespace ast;

    auto loopBody = std::make_unique<Scope>(dummyPosition);
    loopBody->addStatement(std::make_unique<Assignment>(
                               std::make_unique<Name>("x", dummyPosition),
                               std::make_unique<IntLiteral>(1, dummyPosition), dummyPosition)
                          );
    auto program = std::make_unique<Scope>(dummyPosition);
    program->addStatement(std::make_unique<While>(
                           std::make_unique<IntLiteral>(1, dummyPosition),
                           std::move(loopBody), dummyPosition));
    program->addStatement(std::make_unique<Name>("x", dummyPosition));

    Compiler compiler;
    BOOST_CHECK_THROW(program->acceptVisitor(compiler), UndefinedVariable);
}
