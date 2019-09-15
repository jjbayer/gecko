#include <fstream>
#include <iostream>

#include "tokenizer.hpp"
#include "parser.hpp"
#include "compiler.hpp"
#include "executor.hpp"
#include "printvisitor.hpp" // Just for testing

enum ReturnCodes
{
    OK,
    InvalidNumArgs,
    CouldNotOpenFile,
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

    Tokenizer tokenizer;
    auto tokens = tokenizer.tokenize(code);

    auto it = tokens.cbegin(); // will be modified by parse functions
    auto end = tokens.cend();
    auto tree = parseScope(it, end, 0);

    std::cout << "*** Parsed code ***\n";
    ast::PrintVisitor printer;
    tree->acceptVisitor(printer);
    std::cout << "*******************\n\n";

    std::cout << "*** Program output ***\n";
    Compiler compiler;
    tree->acceptVisitor(compiler);
    run(compiler.instructions());
    std::cout << "**********************\n";


    return OK;
}
