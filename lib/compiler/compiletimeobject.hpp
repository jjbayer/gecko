#pragma once
#include "common/object.hpp"
#include "typecreator.hpp"


struct CompileTimeObject
{
    ObjectId id;
    Type type = BasicType::INVALID;
    Type returnType = BasicType::INVALID; // Only for functions
};
