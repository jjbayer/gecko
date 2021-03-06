#include "stdin.hpp"
#include "runtime/instructions.hpp"


namespace ct {


void NextStdin::_generateInstructions(const std::vector<Type> &,
    const std::vector<std::shared_ptr<const CompileTimeObject> > &,
    InstructionVector & instructions,
    std::shared_ptr<CompileTimeObject> returnValue
) const
{
    // Prepare output variable
    const TypeKey typeKey {"Optional", {BasicType::NONE, BasicType::STRING}};
    returnValue->type = typeCreator().getType(typeKey);
    instructions.push_back(
        std::make_shared<ins::SetAllocated>(
            returnValue->id, []{
                return std::make_unique<obj::Tuple<2> >(); }
        )
    );

    // TODO: actually read from given object
    instructions.push_back(std::make_unique<ins::ReadFromStdin>(returnValue->id));
}


} // namespace obj
