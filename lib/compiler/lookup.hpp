#pragma once
#include <unordered_map>
#include <vector>

#include "common/exceptions.hpp"
#include "common/object.hpp"


struct LookupError {};


struct LookupKey
{
    const std::string name = "";
    const std::vector<ObjectType> functionArguments = {};

    bool operator==(const LookupKey & other) const;
};


/// Stolen  from boost
template <class T>
inline void hash_combine(std::size_t& seed, const T& v)
{
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
}


namespace std {

  template <>
  struct hash<LookupKey>
  {
    std::size_t operator()(const LookupKey& key) const
    {
        std::size_t seed {0};
        hash_combine(seed, key.name);
        for(auto arg : key.functionArguments) hash_combine(seed, arg);

        return seed;
    }
  };

}


/// Look up variable in stack of scopes
class Lookup
{
public:

    Lookup();
    void push();
    void pop();

    ObjectId lookup(const LookupKey & key) const;

    std::pair<ObjectId, bool> lookupOrCreate(const LookupKey & key);


    ObjectId freshObjectId() { return mObjectId++; } // TODO: this should not be in this class

    int numObjectIdsUsed() const { return mObjectId; }

private:


    ObjectId mObjectId = 0;
    std::vector<std::unordered_map<LookupKey, ObjectId> > mScopes;
};
