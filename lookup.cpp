#include "lookup.hpp"

Lookup::Lookup()
{
    push(); // There is always global scope
}

void Lookup::pop()
{
    if( mScopes.size() > 1) {
        mScopes.pop_back();
    } else {
        throw std::runtime_error("Trying to pop final scope from lookup");
    }
}

ObjectId Lookup::lookup(const std::__cxx11::string &name) const
{
    for(auto it = mScopes.crbegin(); it != mScopes.crend(); it++) {
        const auto found = it->find(name);
        if( found != it->end() ) {

            return found->second;
        }
    }

    throw UndefinedVariable();  // TODO: context
}

std::pair<ObjectId, bool> Lookup::lookupOrCreate(const std::__cxx11::string &name)
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
