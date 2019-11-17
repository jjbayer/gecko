#pragma once
#include "common/utils.hpp"

#include <unordered_map>
#include <vector>


using Type = unsigned long;


enum BasicType {
    INVALID,
    BOOLEAN,
    INT,
    FLOAT,
    STRING
};


enum MetaType
{
    VALUE,
    FUNCTION,
    STRUCT
};


struct TypeKey
{
    MetaType metaType;
    std::vector<Type> arguments;

    bool operator==(const TypeKey & other) const;
};


namespace std {

  template <>
  struct hash<TypeKey>
  {
    std::size_t operator()(const TypeKey &key) const
    {
        std::size_t seed {0};
        hash_combine(seed, key.metaType);
        for( auto type : key.arguments ) {
            hash_combine(seed, type);
        }

        return seed;
    }
  };

}


class TypeCreator
{
public:
    Type functionType(Type returnType, std::vector<Type> argumentTypes);

private:

    Type issueNewType() { return mNextType++; }

    Type mNextType = 11;

    std::unordered_map<TypeKey, Type> mTypes;

};


