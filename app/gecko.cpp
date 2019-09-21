#include <fstream>
#include <iostream>

#include "tokenizer/tokenizer.hpp"
#include "parser/parser.hpp"
#include "compiler/compiler.hpp"
#include "runtime/executor.hpp"
#include "parser/printvisitor.hpp" // Just for testing

enum ReturnCodes
{
    OK,
    InvalidNumArgs,
    CouldNotOpenFile,
    ProgrammingError,
};

int main(int argc, char ** argv)
{
    if( argc != 2 ) {

        std::cerr << "Need exactly one filename as argument\n";

        return InvalidNumArgs;
    }

    const auto filename = argv[1];
    std::ifstream stream;
    stream.open(filename, std::ios::in);
    if( ! stream.is_open() ) {
        std::cerr << "Could not open file\n";

        return CouldNotOpenFile;
    }

    std::string code(
                std::istreambuf_iterator<char>(stream), {});

    std::unique_ptr<ast::Scope> tree;

    Compiler compiler;

    try {
        Tokenizer tokenizer;
        auto tokens = tokenizer.tokenize(code);

        auto it = tokens.cbegin(); // will be modified by parse functions
        auto end = tokens.cend();
        tree = parseScope(it, end, 0);


        std::cout << "*** Parsed code ***\n";
        ast::PrintVisitor printer;
        tree->acceptVisitor(printer);
        std::cout << "*******************\n\n";

        std::cout << "*** Program output ***\n";
        tree->acceptVisitor(compiler);

    } catch(const ProgammingError & e) {
        std::cerr << e.name() << " at line " << e.mPosition.lineNumber << ", column " << e.mPosition.column << ": "
                  << e.what()
                  << "\n";

        return ReturnCodes::ProgrammingError;
    }

    run(compiler.instructions(), compiler.numObjectIdsUsed());
    std::cout << "**********************\n";


    return OK;
}
