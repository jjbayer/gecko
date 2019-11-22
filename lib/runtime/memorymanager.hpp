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

    /// Will delete anything which is not in the 'keep' list
    void collectGarbage(const std::set<obj::Allocated *> &toBeKept);

private:
    using Map = std::map<obj::Allocated *const, std::unique_ptr<obj::Allocated>>;

    // FIXME: once we have user-defined functions, mAllocatedObjects
    //        should be a stack where each layer contains only objects allocated by current function
    //        collectGarbage() should only remove elements from the top of the stack.
    //        Else, it would delete objects invisible to the current function but still in use
    //        by the caller.
    Map mAllocatedObjects;
};


// TODO: not singleton, make member of executor
MemoryManager &memory();
