#include "scope.hpp"
#include "functionkey.hpp"
#include "functions/function.hpp"


namespace ct {


void Scope::setFunction(std::unique_ptr<Function> function)
{
    const auto key = FunctionGroupKey {
            function->name(),
            function->numTypeParameters(),
            function->numArguments()
    };

    mFunctionGroups[key].setFunction(std::move(function));
}

Function * Scope::findFunction(const FunctionKey &key) const
{
    auto it = mFunctionGroups.find(key);

    if( it == mFunctionGroups.end() ) return nullptr;

    return it->second.find(key);
}

} // namespace ct
