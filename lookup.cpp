#include "lookup.hpp"
#include <sstream>

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
        throw std::runtime_error("Trying to pop final scope from lookup");
    }
}

ObjectId Lookup::lookup(const LookupKey &key) const
{
    for(auto it = mScopes.crbegin(); it != mScopes.crend(); it++) {
        const auto found = it->find(key);
        if( found != it->end() ) {

            return found->second;
        }
    }

    // TODO: readable argument types
    // TODO: key.is_function
    std::stringstream msg;
    msg << key.name;
    if( ! key.functionArguments.empty() ) msg << " with function arguments ";
    for(auto arg : key.functionArguments) msg << arg << " ";

    throw UndefinedVariable({}, msg.str());  // TODO: context
}

std::pair<ObjectId, bool> Lookup::lookupOrCreate(const LookupKey &name)
{
    // TODO: code duplication
    for(auto it = mScopes.crbegin(); it != mScopes.crend(); it++) {
        const auto found = it->find(name);
        if( found != it->end() ) {

            return {found->second, false};
        }
    }

    const auto objectId = freshObjectId();
    mScopes.rbegin()->emplace(name, objectId);

    return {objectId, true};
}

bool LookupKey::operator==(const LookupKey &other) const
{
    if( name != other.name ) return false;
    if( functionArguments.size() != other.functionArguments.size() ) return false;
    for(size_t i = 0; i < functionArguments.size(); i++) {
        if( functionArguments[i] != other.functionArguments[i] ) {

            return false;
        }
    }

    return true;
}
