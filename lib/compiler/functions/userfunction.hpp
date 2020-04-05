#pragma once
#include "function.hpp"


class Instruction;


namespace ct {

class UserFunction: public Function
{
public:

    UserFunction(std::vector<std::shared_ptr<CompileTimeObject> > argumentSlots, Type returnType, InstructionVector instructions)
        : mArgumentSlots(std::move(argumentSlots))
        , mReturnType(returnType)
        , mInstructions(std::move(instructions))
    {
        for(const auto slot : mArgumentSlots) mArgumentTypes.push_back(slot->type);
    }

    virtual std::vector<Type> argumentTypes() const override { return mArgumentTypes; }

    virtual Type returnType() const override { return mReturnType; }

    virtual ~UserFunction() {}

private:

    void _generateInstructions(
        const std::vector<std::shared_ptr<const CompileTimeObject> > & arguments,
        InstructionVector & instructions,
        std::shared_ptr<CompileTimeObject> returnValue
    ) const override;

    std::vector<std::shared_ptr<CompileTimeObject> > mArgumentSlots;
    std::vector<Type> mArgumentTypes;
    Type mReturnType;
    InstructionVector mInstructions;

};

} // namespace ct
