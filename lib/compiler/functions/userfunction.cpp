#include "userfunction.hpp"
#include "runtime/instructions.hpp"


namespace ct {

void UserFunction::_generateInstructions(
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
