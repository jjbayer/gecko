#include "function.hpp"
#include "common/exceptions.hpp"
#include "runtime/instructions.hpp"


namespace ct {

void Function::generateInstructions(const std::vector<Type> &typeParameters,
    const std::vector<std::shared_ptr<const CompileTimeObject> > & arguments,
    InstructionVector & instructions,
    std::shared_ptr<CompileTimeObject> output
) const
{
    std::vector<Type> argumentTypes;
    for(const auto & argument : arguments) argumentTypes.push_back(argument->type);

    if( ! matches({name(), typeParameters, argumentTypes}) ) {

        throw CompilerBug {"Given parameters do not match function requirements"};
    }

    _generateInstructions(typeParameters, arguments, instructions, output);
}

} // namespace ct
