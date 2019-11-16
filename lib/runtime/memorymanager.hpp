#pragma once
#include "common/exceptions.hpp"
#include "objects/allocated.hpp"
#include <memory>
#include <map>

class MemoryManager
{
public:
    obj::Allocated * add(std::unique_ptr<obj::Allocated> ptr);

private:
    using Map = std::map<obj::Allocated *const, std::unique_ptr<obj::Allocated>>;
    Map mAllocatedObjects;
};


// TODO: not singleton, make member of executor
MemoryManager &memory();
