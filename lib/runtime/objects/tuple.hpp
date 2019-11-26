#pragma once
#include <array>
#include "common/object.hpp"
#include "runtime/objects/allocated.hpp"


namespace obj {

template<int N>
class Tuple: public Allocated
{
public:
    std::array<Object, N> data;
};

}
