#pragma once
#include "common/object.hpp"
#include "typecreator.hpp"


struct CompileTimeObject
{
    ObjectId id;
    Type type = BasicType::NONE;
    Type returnType = BasicType::NONE; // Only for functions

    bool isAllocated() const { return type >= BasicType::STRING; }
};
