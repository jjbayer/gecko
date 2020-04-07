#include "memorymanager.hpp"


MemoryManager &memory()
{
    static MemoryManager memoryManager;

    return memoryManager;
}

obj::Allocated *MemoryManager::add(std::unique_ptr<obj::Allocated> ptr)
{
    const auto ret = ptr.get();
    mAllocatedObjects.push_back(std::move(ptr));

    return ret;
}

void MemoryManager::push()
{
    mBarriers.push_back(mAllocatedObjects.size());
}

void MemoryManager::pop()
{
    if( mBarriers.size() < 2 ) {

        throw CompilerBug { "MemoryManager::pop() called too many times" };
    }
    mBarriers.pop_back();
}

void MemoryManager::collectGarbage(const std::set<obj::Allocated *> &toBeKept)
{
    if( mBarriers.empty() ) {

        throw CompilerBug {"MemoryManager::mBarriers must not be empty"};
    }
    const auto barrier = *mBarriers.rbegin();

    const auto n = mAllocatedObjects.size();

    // TODO: Make more efficient by overwriting vector in same iteration
    std::vector<std::unique_ptr<obj::Allocated> > keep;
    for(size_t i = barrier; i < n; i++) {
        auto & obj = mAllocatedObjects[i];
        if( toBeKept.count(obj.get()) ) {
            keep.push_back(std::move(obj));
        }
    }

    mAllocatedObjects.resize(barrier);
    for(auto & ptr : keep) mAllocatedObjects.push_back(std::move(ptr));
}
