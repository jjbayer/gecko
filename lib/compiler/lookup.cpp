#include "lookup.hpp"
#include "functionkey.hpp"
#include "common/utils.hpp"
#include "parser/ast.hpp"


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

bool Lookup::setFunction(std::unique_ptr<Function> function)
{
    return currentScope().setFunction(std::move(function));
}

void Lookup::setType(const std::string &typeString, Type type)
{
    currentScope().mTypes[typeString] = type;
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


const Function * Lookup::lookupFunction(const FunctionKey &key) const
{
    for(auto it = mScopes.crbegin(); it != mScopes.crend(); it++) {
        const auto & scope = *it;
        const auto functionPtr = scope.findFunction(key);
        if( functionPtr ) {

            return functionPtr;
        }
    }

    return nullptr;
}

Type Lookup::lookupType(const std::string &typeString) const
{
    for(auto it = mScopes.crbegin(); it != mScopes.crend(); it++) {
        const auto & scope = *it;
        const auto found = scope.mTypes.find(typeString);
        if( found != scope.mTypes.end() ) {

            return found->second;
        }
    }

    throw LookupError {};
}

} // namespace ct

