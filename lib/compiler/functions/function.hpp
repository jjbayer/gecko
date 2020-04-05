#pragma once
#include <memory>
#include "common/object.hpp"
#include "compiler/compiletimeobject.hpp"


class Instruction;

namespace ct {

class Function
{
public:
    void generateInstructions(
        const std::vector<std::shared_ptr<const CompileTimeObject> > & arguments,
        std::vector<std::unique_ptr<Instruction> > & instructions,
        std::shared_ptr<CompileTimeObject> returnValue
    ) const;

    virtual std::vector<Type> argumentTypes() const = 0;
    virtual Type returnType() const = 0;

private:
    virtual void _generateInstructions(
        const std::vector<std::shared_ptr<const CompileTimeObject> > & arguments,
        std::vector<std::unique_ptr<Instruction> > & instructions,
        std::shared_ptr<CompileTimeObject> returnValue
    ) const = 0;
};

} // namespace ct
