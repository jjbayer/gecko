#pragma once
#include <memory>
#include <string>
#include <vector>

#include "common/utils.hpp"


namespace ct {

class Function;
class FunctionKey;

class FunctionGroupKey
{
public:

    FunctionGroupKey(const std::string & name, size_t numTypeParameters, size_t numArguments);

    /// Allows implicit convertsion from function key to group key
    FunctionGroupKey(const FunctionKey &);

    bool operator==(const FunctionGroupKey & other) const;

    const std::string mName;
    const size_t mNumTypeParameters;
    const size_t mNumArguments;
};


class FunctionGroup
{
public:
    void setFunction(std::unique_ptr<Function> function);

    /// @returns nullptr if no match is found
    Function * find(const FunctionKey & key) const;

private:
    std::vector<std::unique_ptr<Function> > mFunctions;
};

} // namespace ct

namespace std {

  template <>
  struct hash<ct::FunctionGroupKey>
  {
      size_t operator()(const ct::FunctionGroupKey &key) const
      {
          size_t seed {0};
          hash_combine(seed, key.mName);
          hash_combine(seed, key.mNumTypeParameters);
          hash_combine(seed, key.mNumArguments);

          return seed;
      }
  };

}

