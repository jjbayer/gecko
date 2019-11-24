#pragma once
#include "compiletimeobject.hpp"
#include <memory>


class ObjectProvider
{
public:
    std::shared_ptr<CompileTimeObject> createObject(Type type = BasicType::NONE);

    size_t numObjectsIssued() const { return mNextObjectId; }

private:
    size_t mNextObjectId = 0;
};
