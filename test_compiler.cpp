#include "ast.hpp"
#include "printvisitor.hpp"

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

    PrintVisitor visitor;
    program.acceptVisitor(visitor);
}
