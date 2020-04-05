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
        std::vector<std::unique_ptr<Instruction> > & instructions,
        std::shared_ptr<CompileTimeObject> returnValue
    ) const override;

public:
    std::vector<Type> argumentTypes() const override
    {
        return { BasicType::INT };
    }
};


// TODO: templated print function
class PrintString: public ct::Function
{
private:
    void _generateInstructions(
        const std::vector<std::shared_ptr<const CompileTimeObject>> & argumentIds,
        std::vector<std::unique_ptr<Instruction> > & instructions,
        std::shared_ptr<CompileTimeObject> returnValue
    ) const;
public:
    std::vector<Type> argumentTypes() const
    {
        return { BasicType::STRING };
    }
};


} // namespace ct
