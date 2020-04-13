#pragma once
#include "common/utils.hpp"

#include <unordered_map>
#include <vector>


using Type = unsigned long;


enum BasicType {
    NONE,
    BOOLEAN,
    INT,
    FLOAT,
    STRING
};


enum MetaType
{
    VALUE,
    FUNCTION,
    STRUCT,
    ENUM
};


struct TypeKey
{
    MetaType metaType;
    // FIXME: Must include type parameters
    std::vector<Type> arguments;
    std::string fullName = "";

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
    // TODO: only one getType() function
    Type functionType(Type returnType, std::vector<Type> argumentTypes);

    Type structType(const std::string & name);

    Type getType(const TypeKey & key);

    const TypeKey & getTypeKey(Type type) const;

private:


    Type mNextType = 11;

    std::unordered_map<TypeKey, Type> mTypes;
    std::unordered_map<Type, TypeKey> mReverse;
};


TypeCreator &typeCreator();


// TODO: move somewhere else
Type getOptionalType(const TypeCreator & typeCreator, Type type);

