#include "objectprovider.hpp"

CompileTimeObject ObjectProvider::createObject()
{
    return { mNextObjectId++ };
}

CompileTimeObject ObjectProvider::createObject(const Type &type)
{
    return { mNextObjectId++, type };
}
