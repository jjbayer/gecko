#pragma once
#include "common/exceptions.hpp"
#include "objects/allocated.hpp"
#include <memory>
#include <map>
#include <set>

class MemoryManager
{
public:
    obj::Allocated * add(std::unique_ptr<obj::Allocated> ptr);

    /// Add a barrier for garbage collection
    void push();

    /// Remove latest barrier for garbage collection
    void pop();

    /// Will delete anything which is not in the 'keep' list
    void collectGarbage(const std::set<obj::Allocated *> &toBeKept);

private:
    std::vector<std::unique_ptr<obj::Allocated> > mAllocatedObjects;
    std::vector<size_t> mBarriers = {0};
};


// TODO: not singleton, make member of executor
MemoryManager &memory();
