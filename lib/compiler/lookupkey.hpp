#pragma once
#include <optional>
#include <string>
#include <vector>

#include "type.hpp"
#include "common/utils.hpp"


class LookupKey
{
public:
    /// Lookup value types
    LookupKey(const std::string & name);

    /// Lookup function types
    LookupKey(const std::string & name, const std::vector<ValueType> & argumentTypes);

    bool operator==(const LookupKey & other) const;

    bool isFunction() const { return mArgumentTypes.has_value(); }

    const std::string mName;
    const std::optional<std::vector<ValueType> > mArgumentTypes;

};


namespace std {

  template <>
  struct hash<LookupKey>
  {
    std::size_t operator()(const LookupKey& key) const
    {
        std::size_t seed {0};
        hash_combine(seed, key.mName);
        if( key.isFunction() ) {
            hash_combine(seed, true);
            for(auto arg : *key.mArgumentTypes) hash_combine(seed, arg);
        } else {
            hash_combine(seed, false);
        }

        return seed;
    }
  };

}
