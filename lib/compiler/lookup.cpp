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
        throw std::runtime_error("Trying to pop final scope from lookup");
    }
}


void Lookup::set(const LookupKey &key, std::shared_ptr<CompileTimeObject> object)
{
    auto & top = *mScopes.rbegin();
    top[key] = object;
}


std::shared_ptr<CompileTimeObject> Lookup::lookup(const LookupKey &key) const
{
    for(auto it = mScopes.crbegin(); it != mScopes.crend(); it++) {
        const auto found = it->find(key);
        if( found != it->end() ) {

            return found->second;
        }
    }

    throw LookupError {};
}




