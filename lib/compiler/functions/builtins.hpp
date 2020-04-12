#pragma once
#include "function.hpp"
#include "compiler/typecreator.hpp"


namespace ct {

// TODO: remove printInt
class PrintInt: public PlainFunction
{
public:
    PrintInt(const FunctionKey & key): PlainFunction(key) {}
private:
    void _generateInstructions(const std::vector<Type> &typeParameters, const std::vector<std::shared_ptr<const CompileTimeObject> > &arguments, InstructionVector &instructions, std::shared_ptr<CompileTimeObject> returnValue) const;
};


// TODO: templated print function
class PrintString: public PlainFunction
{
public:
    PrintString(const FunctionKey & key): PlainFunction(key) {}

private:
    void _generateInstructions(
        const std::vector<Type> & typeParameters,
        const std::vector<std::shared_ptr<const CompileTimeObject>> & arguments,
        InstructionVector & instructions,
        std::shared_ptr<CompileTimeObject> returnValue
    ) const;
};


} // namespace ct
