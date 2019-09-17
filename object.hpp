#pragma once
#include <stdint.h>

using ObjectId = int;


/// Runtime objects do not have an ObjectType
enum ObjectType
{
    BOOLEAN = 0,
    INT = 1,
    FLOAT = 2,
    STRING = 3
};


union Object {
    bool as_bool;
    int64_t as_int;  // TODO: what  if 32 bit arch? Check sizeof types statically
    double as_float;
};
