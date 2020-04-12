#include "objectprovider.hpp"

namespace ct {

std::shared_ptr<CompileTimeObject> ObjectProvider::createObject(Type type)
{
    auto object = std::make_shared<CompileTimeObject>();
    object->id = mNextObjectId++;
    object->type = type;

    return object;
}

} // namespace ct
