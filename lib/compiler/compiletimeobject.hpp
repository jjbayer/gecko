#pragma once
#include "common/object.hpp"
#include "type.hpp"


struct CompileTimeObject
{
    ObjectId id;
    Type type = Type { MetaType::Invalid };
};
