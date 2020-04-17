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
    returnValue->type = typeCreator().getType({ "List", typeParameters });
    const auto isAllocated = returnValue->isAllocated();
    instructions.push_back(
        std::make_unique<ins::SetAllocated>(
            returnValue->id,
            [isAllocated]() { return obj::makeList(isAllocated); }
        )
                );
}

bool ListLength::matches(const FunctionKey &key) const
{
    if( key.mTypeParameters.size() == 0 && key.mArgumentTypes.size() == 1 ) {

        const auto & typeKey = typeCreator().getTypeKey(key.mArgumentTypes[0]);

        return typeKey.name == "List";
    }

    return false;
}

void ListLength::_generateInstructions(const std::vector<Type> &typeParameters, const std::vector<std::shared_ptr<const CompileTimeObject> > &arguments, InstructionVector &instructions, std::shared_ptr<CompileTimeObject> returnValue) const
{
    returnValue->type = BasicType::INT;
    auto list = arguments.at(0);
    instructions.push_back(std::make_unique<ins::GetListLength>(list->id, returnValue->id));
}

bool ListAppend::matches(const FunctionKey &key) const
{
    if( key.mTypeParameters.size() == 0 && key.mArgumentTypes.size() == 2 ) {

        const auto & typeKey = typeCreator().getTypeKey(key.mArgumentTypes[0]);

        if( typeKey.name == "List" ) {
            const auto containedType = typeKey.typeParameters.at(0);

            return containedType == key.mArgumentTypes[1];
        }
    }

    return false;
}

void ListAppend::_generateInstructions(const std::vector<Type> &, const std::vector<std::shared_ptr<const CompileTimeObject> > &arguments, InstructionVector & instructions, std::shared_ptr<CompileTimeObject>) const
{
    instructions.push_back(std::make_unique<ins::AppendToList>(arguments.at(0)->id, arguments.at(1)->id));
}


} // namespace ct

