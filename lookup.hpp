#pragma once
#include <unordered_map>
#include <vector>

#include "exceptions.hpp"
#include "instructions.hpp" // TODO: replace by "object.hpp"

/// Look up variable in stack of scopes
class Lookup
{
public:
    Lookup();
    void push()
    {
        mScopes.emplace_back();
    }
    void pop();

    ObjectId lookup(const std::string & name) const;

    std::pair<ObjectId, bool> lookupOrCreate(const std::string & name);


    ObjectId freshObjectId() { return mObjectId++; } // TODO: this should not be in this class

private:


    ObjectId mObjectId = 0;
    std::vector<std::unordered_map<std::string, ObjectId> > mScopes;
};
