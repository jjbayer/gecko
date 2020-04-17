#pragma once
#include <array>
#include <bitset>
#include "common/exceptions.hpp"
#include "common/object.hpp"
#include "runtime/objects/allocated.hpp"


namespace obj {


template<size_t N>
class Tuple: public Allocated
{
public:

    std::array<Object, N> data;

    std::vector<const Allocated *> children() const
    {
        throw MissingFeature {"Tuple::children()" };
    }

private:
    const std::bitset<N> mIsAllocated;
};


}
