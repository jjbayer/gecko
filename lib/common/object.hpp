#pragma once
#include <stdint.h>

namespace obj { class Function; }

using ObjectId = int;


/// Runtime objects do not have an ObjectType
enum ObjectType
{
    INVALID = 0,
    BOOLEAN = 1,
    INT = 2,
    FLOAT = 3,
    STRING = 4
};


union Object {
    bool as_bool;
    int64_t as_int;  // TODO: what  if 32 bit arch? Check sizeof types statically
    double as_float;
    obj::Function * as_function_ptr;
};
