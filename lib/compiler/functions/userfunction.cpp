#include "userfunction.hpp"
#include "runtime/instructions.hpp"


namespace ct {

UserFunction::UserFunction(const FunctionKey &key, std::vector<std::shared_ptr<CompileTimeObject> > argumentSlots, std::shared_ptr<const CompileTimeObject> returnObject, InstructionVector instructions)
    : PlainFunction(key)
    , mArgumentSlots(std::move(argumentSlots))
    , mReturnObject(returnObject)
    , mInstructions(std::move(instructions))
{
    for(const auto & slot : mArgumentSlots) mArgumentTypes.push_back(slot->type);
}

void UserFunction::_generateInstructions(const std::vector<Type> &,
        const std::vector<std::shared_ptr<const CompileTimeObject> > & arguments,
        InstructionVector & instructions,
        std::shared_ptr<CompileTimeObject> returnValue
        ) const
{
    // NOTE: Parent class already checks if vectors have same length

    // Copy arguments to argument slots
    for(size_t i = 0; i < arguments.size(); i++) {
        auto arg = arguments.at(i);
        auto slot = mArgumentSlots.at(i);
        instructions.emplace_back(std::make_shared<ins::Copy>(arg->id, slot->id));
    }

    for(const auto & instruction : mInstructions) {
        instructions.emplace_back(instruction);
    }

    // Target object is same as latest object
    *returnValue = *mReturnObject;
}

} // namespace ct
