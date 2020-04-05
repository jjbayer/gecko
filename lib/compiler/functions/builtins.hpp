#pragma once
#include "function.hpp"
#include "compiler/typecreator.hpp"


namespace ct {

// TODO: remove printInt
class PrintInt: public ct::Function
{
private:

    void _generateInstructions(
        const std::vector<std::shared_ptr<const CompileTimeObject> > & argumentIds,
        InstructionVector & instructions,
        std::shared_ptr<CompileTimeObject> returnValue
    ) const override;

public:
    std::vector<Type> argumentTypes() const override
    {
        return { BasicType::INT };
    }

    Type returnType() const override { return BasicType::NONE; }
};


// TODO: templated print function
class PrintString: public ct::Function
{
private:
    void _generateInstructions(
        const std::vector<std::shared_ptr<const CompileTimeObject>> & argumentIds,
        InstructionVector & instructions,
        std::shared_ptr<CompileTimeObject> returnValue
    ) const;
public:
    std::vector<Type> argumentTypes() const
    {
        return { BasicType::STRING };
    }

    Type returnType() const override { return BasicType::NONE; }
};


} // namespace ct
