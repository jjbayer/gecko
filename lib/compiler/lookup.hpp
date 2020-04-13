#pragma once
#include <memory>
#include <unordered_map>
#include <vector>

#include "typecreator.hpp"
#include "scope.hpp"
#include "common/exceptions.hpp"
#include "common/object.hpp"
#include "compiletimeobject.hpp"
#include "functions/function.hpp"



namespace ct {

class FunctionKey;

class LookupError {};


/// Look up variable in stack of scopes
class Lookup
{
public:

    Lookup();
    void push();
    void pop();

    std::shared_ptr<CompileTimeObject> lookupObject(const std::string & key) const;
    const Function * lookupFunction(const FunctionKey & key) const;
    Type lookupType(const std::string & typeString) const;

    void setObject(const std::string & key, std::shared_ptr<CompileTimeObject> object);
    void setFunction(std::unique_ptr<Function> function);
    void setType(const std::string & typeString, Type typeId);

    // TODO: iterator
    const std::vector<Scope> & scopes() const { return mScopes; }

private:

    Scope & currentScope() {  return *mScopes.rbegin(); }

    std::vector<Scope> mScopes;
};

} // namespace ct
