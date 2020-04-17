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


struct TypeKey
{
    std::string name;
    std::vector<Type> typeParameters = {};

    bool operator==(const TypeKey & other) const;

    std::string toString() const;
};


namespace std {

  template <>
  struct hash<TypeKey>
  {
    std::size_t operator()(const TypeKey &key) const
    {
        std::size_t seed {0};
        for( auto type : key.typeParameters ) {
            hash_combine(seed, type);
        }

        return seed;
    }
  };

}


class TypeCreator
{
public:

    TypeCreator();

    Type getType(const TypeKey & key);

    const TypeKey & getTypeKey(Type type) const;

private:


    Type mNextType = 11;

    std::unordered_map<TypeKey, Type> mTypes;
    std::unordered_map<Type, TypeKey> mReverse {
        {NONE, {"None"}},
        {BOOLEAN, {"Bool"}},
        {INT, {"Int"}},
        {FLOAT, {"Float"}},
        {STRING, {"String"}},
    };
};


TypeCreator &typeCreator();


// TODO: move somewhere else
Type getOptionalType(const TypeCreator & typeCreator, Type type);

