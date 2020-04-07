#pragma once
#include <memory>
#include <unordered_map>
#include <vector>

#include "typecreator.hpp"
#include "common/exceptions.hpp"
#include "common/object.hpp"
#include "lookupkey.hpp"
#include "compiletimeobject.hpp"
#include "functions/function.hpp"



namespace ct {

struct LookupError {};


class LookupScope
{
public:
    std::unordered_map<std::string, std::shared_ptr<CompileTimeObject> > mObjects;
    std::unordered_map<FunctionKey, std::unique_ptr<Function> > mFunctions;
    std::unordered_map<std::string, Type> mTypes;
};


/// Look up variable in stack of scopes
class Lookup
{
public:

    Lookup();
    void push();
    void pop();

    std::shared_ptr<CompileTimeObject> lookupObject(const std::string & key) const;
    const Function * lookupFunction(const FunctionKey & key) const;
    Type lookupType(const std::string & typeName) const;

    void setObject(const std::string & key, std::shared_ptr<CompileTimeObject> object);
    bool setFunction(const FunctionKey & key, std::unique_ptr<Function> function);
    void setType(const std::string & typeName, Type type);

    // TODO: iterator
    const std::vector<LookupScope> & scopes() const { return mScopes; }

private:

    LookupScope & currentScope() {  return *mScopes.rbegin(); }

    std::vector<LookupScope> mScopes;
};

} // namespace ct
