#include "builtins.hpp"
#include "runtime/instructions.hpp"
#include "runtime/objects/list.hpp"


namespace ct {

void PrintInt::_generateInstructions(
    const std::vector<Type> & ,
    const std::vector<std::shared_ptr<const CompileTimeObject> > & arguments,
    InstructionVector & instructions,
    std::shared_ptr<CompileTimeObject> returnValue
) const
{
    instructions.push_back(std::make_unique<ins::PrintInt>(arguments.at(0)->id));
    returnValue->type = BasicType::NONE;
}


void PrintString::_generateInstructions(
    const std::vector<Type> &,
    const std::vector<std::shared_ptr<const CompileTimeObject> > & arguments,
    InstructionVector & instructions,
    std::shared_ptr<CompileTimeObject> returnValue
) const
{
    instructions.push_back(std::make_unique<ins::PrintString>(arguments.at(0)->id));
    returnValue->type = BasicType::NONE;
}

void ListCtor::_generateInstructions(const std::vector<Type> &typeParameters, const std::vector<std::shared_ptr<const CompileTimeObject> > &, InstructionVector &instructions, std::shared_ptr<CompileTimeObject> returnValue)
    const
{
    returnValue->type = typeParameters.at(0);
    const auto isAllocated = returnValue->isAllocated();
    instructions.push_back(
        std::make_unique<ins::SetAllocated>(
            returnValue->id,
            [isAllocated]() { return obj::makeList(isAllocated); }
        )
    );
}


} // namespace ct

