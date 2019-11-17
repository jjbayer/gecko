#pragma once
#include <unordered_map>
#include <vector>

#include "common/exceptions.hpp"
#include "common/object.hpp"
#include "lookupkey.hpp"
#include "compiletimeobject.hpp"


struct LookupError {};


/// Look up variable in stack of scopes
class Lookup
{
public:

    using Map = std::unordered_map<LookupKey, CompileTimeObject>;

    Lookup();
    void push();
    void pop();

    CompileTimeObject lookup(const LookupKey & key) const;

    void set(const LookupKey & key, CompileTimeObject object);

    // TODO: iterator
    const std::vector<Map> & scopes() const { return mScopes; }

private:

    Map & currentScope() {  return *mScopes.rbegin(); }

    std::vector<Map> mScopes;
};
