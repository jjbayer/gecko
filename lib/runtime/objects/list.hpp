#pragma once
#include "allocated.hpp"
#include "common/object.hpp"
#include <memory>


namespace obj {


class List: public Allocated
{
public:
    std::vector<Object> mItems;
};


class ListOfSimple: public List
{
public:
    std::vector<const Allocated *> children() const override { return {}; }
};


class ListOfAllocated: public List
{
public:
    std::vector<const Allocated *> children() const;
};


std::unique_ptr<List> makeList(bool isAllocated);


} // namespace obj
