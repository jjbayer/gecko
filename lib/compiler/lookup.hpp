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

    Lookup();
    void push();
    void pop();

    CompileTimeObject lookup(const LookupKey & key) const;

    void set(const LookupKey & key, CompileTimeObject object);

private:

    std::unordered_map<LookupKey, CompileTimeObject> & currentScope() {  return *mScopes.rbegin(); }

    std::vector<std::unordered_map<LookupKey, CompileTimeObject> > mScopes;
};
