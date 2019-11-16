#pragma once
#include "compiletimeobject.hpp"


class ObjectProvider
{
public:
    CompileTimeObject createObject();

    CompileTimeObject createObject(const Type & type);

    size_t numObjectsIssued() const { return mNextObjectId; }

private:
    size_t mNextObjectId = 0;
};
