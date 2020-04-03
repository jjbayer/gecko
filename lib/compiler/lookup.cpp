#include "lookup.hpp"
#include "common/utils.hpp"


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


void Lookup::set(const LookupKey &key, std::shared_ptr<CompileTimeObject> object)
{
    auto & top = *mScopes.rbegin();
    top.mObjects[key] = object;
}

void Lookup::setType(const std::string & typeName, Type type)
{
    auto & top = *mScopes.rbegin();
    top.mTypes[typeName] = type;
}


std::shared_ptr<CompileTimeObject> Lookup::lookup(const LookupKey &key) const
{
    for(auto it = mScopes.crbegin(); it != mScopes.crend(); it++) {
        const auto & scope = *it;
        const auto found = scope.mObjects.find(key);
        if( found != scope.mObjects.end() ) {

            return found->second;
        }
    }

    return nullptr;
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


