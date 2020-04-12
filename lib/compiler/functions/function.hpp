#pragma once
#include <memory>
#include "common/object.hpp"
#include "compiler/compiletimeobject.hpp"
#include "compiler/functionkey.hpp"


class Instruction;
using InstructionVector = std::vector<std::shared_ptr<const Instruction> >;


namespace ct {


class Function
{
public:
    void generateInstructions(
        const std::vector<Type> & typeParameters,
        const std::vector<std::shared_ptr<const CompileTimeObject> > & arguments,
        InstructionVector & instructions,
        std::shared_ptr<CompileTimeObject> returnValue
    ) const;

    virtual const std::string & name() const = 0;
    virtual size_t numTypeParameters() const = 0;
    virtual size_t numArguments() const = 0;

    virtual bool matches(const FunctionKey &) const = 0;

    virtual ~Function() {}

private:
    virtual void _generateInstructions(
        const std::vector<Type> & typeParameters,
        const std::vector<std::shared_ptr<const CompileTimeObject> > & arguments,
        InstructionVector & instructions,
        std::shared_ptr<CompileTimeObject> returnValue
    ) const = 0;
};


/// Function with a one to one match to a function key
class PlainFunction: public Function
{
public:

    PlainFunction(const FunctionKey & key): mFunctionKey(key) {}

    const std::string &name() const { return mFunctionKey.mName; }
    size_t numTypeParameters() const { return mFunctionKey.mTypeParameters.size(); }
    size_t numArguments() const { return mFunctionKey.mArgumentTypes.size(); }
    bool matches(const FunctionKey & key) const { return key == mFunctionKey; }

private:
    FunctionKey mFunctionKey;
};


} // namespace ct
