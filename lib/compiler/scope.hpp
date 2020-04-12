#pragma once
#include "functiongroup.hpp"
#include "typecreator.hpp"

#include <memory>
#include <unordered_map>
#include <string>


namespace ct {


class CompileTimeObject;
class Function;


class Scope
{
public:

    /// @return false if function already exists
    bool setFunction(std::unique_ptr<Function> function);

    /// @return nullptr if no matching function was found
    Function * findFunction(const FunctionKey & key) const;

    std::unordered_map<std::string, std::shared_ptr<CompileTimeObject> > mObjects;
    std::unordered_map<FunctionGroupKey, FunctionGroup > mFunctionGroups;
    std::unordered_map<std::string, Type> mTypes;
};

} // namespace ct
