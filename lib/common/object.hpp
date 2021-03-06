#pragma once
#include <stdint.h>
#include <stddef.h>

namespace obj { class Allocated; }

using ObjectId = size_t;


union Object {
    bool as_boolean;
    int64_t as_int;  // TODO: what  if 32 bit arch? Check sizeof types statically
    double as_float;
    obj::Allocated * as_ptr;
};
