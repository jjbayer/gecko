#include "function.hpp"
#include "common/exceptions.hpp"
#include "runtime/instructions.hpp"


namespace ct {

void Function::generateInstructions(
    const std::vector<std::shared_ptr<const CompileTimeObject> > & arguments,
    InstructionVector & instructions,
    std::shared_ptr<CompileTimeObject> output
) const
{
    const auto argTypes = argumentTypes();

    if( arguments.size() != argTypes.size() ) {
        throw CompilerBug {"arguments length != argument types length"};
    }

    for(size_t i = 0; i < arguments.size(); i++) {
        if( arguments.at(i)->type != argTypes.at(i) ) {
            throw CompilerBug {"given argument types do not match function signature"};
        }
    }

    _generateInstructions(arguments, instructions, output);
}

} // namespace ct
