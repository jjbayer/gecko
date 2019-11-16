#pragma once
#include "typecreator.hpp"
#include "common/utils.hpp"

#include <optional>
#include <string>
#include <vector>



class LookupKey
{
public:
    /// Lookup value types
    LookupKey(const std::string & name);

    /// Lookup function types
    LookupKey(const std::string & name, const std::vector<Type> & argumentTypes);

    bool operator==(const LookupKey & other) const;

    bool isFunction() const { return mArgumentTypes.has_value(); }

    const std::string mName;
    const std::optional<std::vector<Type> > mArgumentTypes;

};


namespace std {

  template <>
  struct hash<LookupKey>
  {
      size_t operator()(const LookupKey &key) const
      {
          size_t seed {0};
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
