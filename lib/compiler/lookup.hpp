#pragma once
#include <memory>
#include <unordered_map>
#include <vector>

#include "typecreator.hpp"
#include "common/exceptions.hpp"
#include "common/object.hpp"
#include "lookupkey.hpp"
#include "compiletimeobject.hpp"


struct LookupError {};


class LookupScope
{
public:
    std::unordered_map<LookupKey, std::shared_ptr<CompileTimeObject> > mObjects;
    std::unordered_map<std::string, Type> mTypes;
};


/// Look up variable in stack of scopes
class Lookup
{
public:

    Lookup();
    void push();
    void pop();

    std::shared_ptr<CompileTimeObject> lookup(const LookupKey & key) const;
    Type lookupType(const std::string & typeName) const;

    void set(const LookupKey & key, std::shared_ptr<CompileTimeObject> object);
    void setType(const std::string & typeName, Type type);

    // TODO: iterator
    const std::vector<LookupScope> & scopes() const { return mScopes; }

private:

    LookupScope & currentScope() {  return *mScopes.rbegin(); }

    std::vector<LookupScope> mScopes;
};
