#include "lookup.hpp"
#include "common/utils.hpp"


namespace ct {

Lookup::Lookup()
{
    push(); // There is always global scope
}

void Lookup::push()
{
    mScopes.emplace_back();
}

void Lookup::pop()
{
    if( mScopes.size() > 1) {
        mScopes.pop_back();
    } else {
        throw CompilerBug {"Trying to pop final scope from lookup"};
    }
}


void Lookup::setObject(const std::string &key, std::shared_ptr<CompileTimeObject> object)
{
    // TODO: disallow shadowing
    currentScope().mObjects[key] = object;
}

void Lookup::setFunction(const FunctionKey & key, std::unique_ptr<Function> function)
{
    currentScope().mFunctions[key] = std::move(function);
}

void Lookup::setType(const std::string & typeName, Type type)
{
    currentScope().mTypes[typeName] = type;
}


std::shared_ptr<CompileTimeObject> Lookup::lookupObject(const std::string &key) const
{
    for(auto it = mScopes.crbegin(); it != mScopes.crend(); it++) {
        const auto & scope = *it;
        const auto found = scope.mObjects.find(key);
        if( found != scope.mObjects.end() ) {

            return found->second;
        }
    }

    return nullptr; // TODO: inconsistent with other lookup functions
}


const Function & Lookup::lookupFunction(const FunctionKey &key) const
{
    for(auto it = mScopes.crbegin(); it != mScopes.crend(); it++) {
        const auto & scope = *it;
        const auto found = scope.mFunctions.find(key);
        if( found != scope.mFunctions.end() ) {

            return *(found->second);
        }
    }

    throw LookupError {};
}

Type Lookup::lookupType(const std::string & typeName) const
{
    for(auto it = mScopes.crbegin(); it != mScopes.crend(); it++) {
        const auto & scope = *it;
        const auto found = scope.mTypes.find(typeName);
        if( found != scope.mTypes.end() ) {

            return found->second;
        }
    }

    throw LookupError {};
}

} // namespace ct

