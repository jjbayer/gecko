#include "type.hpp"

bool Type::operator==(const Type &other) const
{
    // 1. check metatype
    if( metaType != other.metaType ) return false;

    // 2. check value type
    if( returnType != other.returnType ) return false;

    // 3. check argument types
    if( argumentTypes.size() != other.argumentTypes.size()) {

        return false;
    }
    for(size_t i = 0; i < argumentTypes.size(); i++) {
        if( argumentTypes[i] != other.argumentTypes[i]) {

            return false;
        }
    }

    return true;
}

bool Type::operator!=(const Type &other) const
{
    return ! (*this == other);
}

bool Type::operator==(const ValueType &other) const
{
    return metaType == MetaType::Value && returnType == other;
}

bool Type::operator!=(const ValueType &other) const
{
    return metaType != MetaType::Value || returnType != other;
}
