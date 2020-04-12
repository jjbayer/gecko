#include "builtins.hpp"
#include "runtime/instructions.hpp"


namespace ct {

void PrintInt::_generateInstructions(
    const std::vector<Type> & typeParameters,
    const std::vector<std::shared_ptr<const CompileTimeObject> > & arguments,
    InstructionVector & instructions,
    std::shared_ptr<CompileTimeObject> returnValue
) const
{
    instructions.push_back(std::make_unique<ins::PrintInt>(arguments.at(0)->id));
    returnValue->type = BasicType::NONE;
}


void PrintString::_generateInstructions(
    const std::vector<Type> &typeParameters,
    const std::vector<std::shared_ptr<const CompileTimeObject> > & arguments,
    InstructionVector & instructions,
    std::shared_ptr<CompileTimeObject> returnValue
) const
{
    instructions.push_back(std::make_unique<ins::PrintString>(arguments.at(0)->id));
    returnValue->type = BasicType::NONE;
}

} // namespace ct

