#pragma once
#include <vector>

enum class MetaType {
    Invalid,
    Value,
    Function
};

enum class ValueType {
    INVALID = 0,
    BOOLEAN = 1,
    INT = 2,
    FLOAT = 3,
    STRING = 4
};

/// Type for compile time objects
struct Type
{
    MetaType metaType;
    ValueType returnType = ValueType::INVALID;
    std::vector<Type> argumentTypes = {}; // for functions

    bool operator==(const Type & other) const;
    bool operator!=(const Type & other) const;

    /// Most types are values types, so here's a shortcut
    /// for readability in caller code
    bool operator==(const ValueType & other) const;
    bool operator!=(const ValueType & other) const;


};
