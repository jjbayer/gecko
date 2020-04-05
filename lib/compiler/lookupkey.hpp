#pragma once
#include "typecreator.hpp"
#include "common/utils.hpp"

#include <optional>
#include <string>
#include <vector>



class FunctionKey
{
public:
    /// Lookup value types
    FunctionKey(const std::string & name);

    /// Lookup function types
    FunctionKey(const std::string & name, const std::vector<Type> & argumentTypes);

    bool operator==(const FunctionKey & other) const;

    const std::string mName;
    const std::vector<Type> mArgumentTypes;

};


namespace std {

  template <>
  struct hash<FunctionKey>
  {
      size_t operator()(const FunctionKey &key) const
      {
          size_t seed {0};
          hash_combine(seed, key.mName);
          for(auto arg : key.mArgumentTypes) hash_combine(seed, arg);

          return seed;
      }
  };

}
