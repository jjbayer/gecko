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


class ListCtor: public Function
{
    const std::string &name() const override { static const std::string name {"List"}; return name; }
    size_t numTypeParameters() const override { return 1; }
    size_t numArguments() const override { return 0; }
    bool matches(const FunctionKey & key) const override
    {
        return ( key.mTypeParameters.size() == 1 && key.mArgumentTypes.empty() );
    }

private:
    void _generateInstructions(
        const std::vector<Type> &typeParameters,
        const std::vector<std::shared_ptr<const CompileTimeObject> > &arguments,
        InstructionVector &instructions,
        std::shared_ptr<CompileTimeObject> returnValue
    ) const override;
};


class ListLength: public Function
{
    const std::string &name() const override { static const std::string name {"length"}; return name; }
    size_t numTypeParameters() const override { return 0; }
    size_t numArguments() const override { return 1; }
    bool matches(const FunctionKey & key) const override;

private:
    void _generateInstructions(
        const std::vector<Type> &typeParameters,
        const std::vector<std::shared_ptr<const CompileTimeObject> > &arguments,
        InstructionVector &instructions,
        std::shared_ptr<CompileTimeObject> returnValue
    ) const override;
};


class ListAppend: public Function
{
    const std::string &name() const override { static const std::string name {"append"}; return name; }
    size_t numTypeParameters() const override { return 0; }
    size_t numArguments() const override { return 2; }
    bool matches(const FunctionKey & key) const override;

private:
    void _generateInstructions(
        const std::vector<Type> &typeParameters,
        const std::vector<std::shared_ptr<const CompileTimeObject> > &arguments,
        InstructionVector &instructions,
        std::shared_ptr<CompileTimeObject> returnValue
    ) const override;
};



} // namespace ct
