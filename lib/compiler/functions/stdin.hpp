#pragma once
#include "function.hpp"


namespace ct {


class NextStdin: public ct::Function
{
private:

    void _generateInstructions(
        const std::vector<std::shared_ptr<const CompileTimeObject> > & argumentIds,
        std::vector<std::unique_ptr<Instruction> > & instructions,
        std::shared_ptr<CompileTimeObject> returnValue
    ) const override;

public:
    std::vector<Type> argumentTypes() const override;
};

} // namespace ct

