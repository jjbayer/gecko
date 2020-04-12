#pragma once
#include "function.hpp"


class Instruction;


namespace ct {

class UserFunction: public PlainFunction // TODO: allow template user  functions
{
public:

    UserFunction(
        const FunctionKey & key,
        std::vector<std::shared_ptr<CompileTimeObject> > argumentSlots, std::shared_ptr<const CompileTimeObject> returnObject, InstructionVector instructions);

private:

    void _generateInstructions(
        const std::vector<Type> &,
        const std::vector<std::shared_ptr<const CompileTimeObject> > & arguments,
        InstructionVector & instructions,
        std::shared_ptr<CompileTimeObject> returnValue
    ) const override;

    std::vector<std::shared_ptr<CompileTimeObject> > mArgumentSlots;
    std::vector<Type> mArgumentTypes;
    std::shared_ptr<const CompileTimeObject> mReturnObject;
    InstructionVector mInstructions;

};

} // namespace ct
