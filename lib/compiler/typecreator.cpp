#include "typecreator.hpp"


bool TypeKey::operator==(const TypeKey &other) const
{
    if( metaType != other.metaType ) return false;

    if( arguments.size() != other.arguments.size() ) return false;

    for(size_t i = 0; i < arguments.size(); i++) {
        if( arguments[i] != other.arguments[i] ) return false;
    }

    return true;
}
