#include "functiongroup.hpp"

#include "functionkey.hpp"
#include "functions/function.hpp"


namespace ct {


FunctionGroupKey::FunctionGroupKey(const std::string &name, size_t numTypeParameters, size_t numArguments)
    : mName(name)
    , mNumTypeParameters(numTypeParameters)
    , mNumArguments(numArguments)
{}

FunctionGroupKey::FunctionGroupKey(const FunctionKey & functionKey)
    : mName(functionKey.mName)
    , mNumTypeParameters(functionKey.mTypeParameters.size())
    , mNumArguments(functionKey.mArgumentTypes.size())
{}

bool FunctionGroupKey::operator==(const FunctionGroupKey &other) const
{
    return mName == other.mName && mNumTypeParameters == other.mNumTypeParameters && mNumArguments == other.mNumArguments;
}

void FunctionGroup::setFunction(std::unique_ptr<Function> function)
{
    mFunctions.push_back(std::move(function));
}


Function *FunctionGroup::find(const FunctionKey &key) const
{
    for(const auto & function : mFunctions) {
        if( function->matches(key) ) return function.get();
    }

    return nullptr;
}


} // namespace ct
