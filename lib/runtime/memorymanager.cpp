#include "memorymanager.hpp"


MemoryManager &memory()
{
    static MemoryManager memoryManager;

    return memoryManager;
}

obj::Allocated *MemoryManager::add(std::unique_ptr<obj::Allocated> ptr)
{
    auto ret = ptr.get();
    auto [it, emplaced] = mAllocatedObjects.emplace(ret, std::move(ptr));
            if( ! emplaced ) {
        throw CompilerBug("Address already managed");
    }

    return ret;
}

void MemoryManager::collectGarbage(const std::set<obj::Allocated *> &toBeKept)
{
    Map keep;
    for(auto ptr : toBeKept) {
        auto it = mAllocatedObjects.find(ptr);
        if( it != mAllocatedObjects.end() ) {
            keep.insert(std::move(*it));
        }
    }

    std::swap(keep, mAllocatedObjects);
}
