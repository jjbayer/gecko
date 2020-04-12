#pragma once
#include "function.hpp"


namespace ct {


class NextStdin: public PlainFunction
{
public:

    NextStdin(const FunctionKey & key): PlainFunction(key) {}

private:
    void _generateInstructions(
        const std::vector<Type> & typeParameters,
        const std::vector<std::shared_ptr<const CompileTimeObject> > & arguments,
        InstructionVector & instructions,
        std::shared_ptr<CompileTimeObject> returnValue
    ) const override;
};

} // namespace ct

