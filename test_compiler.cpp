#include "ast.hpp"
#include  "compiler.hpp"
#include "executor.hpp"
#include "printvisitor.hpp"

#include <iostream>

int main()
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

    std::cout <<  "AST serialized as code: \n";
    PrintVisitor visitor;
    program.acceptVisitor(visitor);

    std::cout << "BEGIN Output of executed program: \n";
    Compiler compiler;
    program.acceptVisitor(compiler);
    run(compiler.instructions());
    std::cout << "END\n";

    return 0;
}
