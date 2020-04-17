#include "list.hpp"

namespace obj {


std::vector<const Allocated *> ListOfAllocated::children() const
{
    std::vector<const Allocated *> ret;

    for(auto object : mItems) {
        ret.push_back(object.as_ptr);
    };

    return ret;
}

std::unique_ptr<List> makeList(bool isAllocated)
{
    if( isAllocated ) return std::make_unique<ListOfAllocated>();

    return std::make_unique<ListOfSimple>();
}

} // namespace obj
