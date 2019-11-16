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

Type TypeCreator::functionType(Type returnType, std::vector<Type> argumentTypes)
{
    argumentTypes.push_back(returnType);
    TypeKey key { MetaType::FUNCTION, argumentTypes };
    auto [it, created] = mTypes.emplace(key, mNextType);
            if( created ) {
        mNextType++;
    }

    return it->second;
}
