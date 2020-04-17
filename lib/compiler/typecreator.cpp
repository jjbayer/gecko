#include "typecreator.hpp"
#include "common/exceptions.hpp"


bool TypeKey::operator==(const TypeKey &other) const
{
    if( typeParameters.size() != other.typeParameters.size() ) return false;

    for(size_t i = 0; i < typeParameters.size(); i++) {
        if( typeParameters[i] != other.typeParameters[i] ) return false;
    }

    if( name != other.name) return false;

    return true;
}


const TypeKey &TypeCreator::getTypeKey(Type type) const
{
    return mReverse.at(type);
}

Type TypeCreator::getType(const TypeKey &key)
{
    auto [it, created] = mTypes.emplace(key, mNextType);
    if( created ) {
        mReverse[mNextType++] = key;
    }

    return it->second;
}

Type getOptionalType(const TypeCreator &typeCreator, Type type)
{
    const auto & typeKey = typeCreator.getTypeKey(type);
    if( typeKey.name != "Optional"
            || typeKey.typeParameters.size() != 2
            || typeKey.typeParameters[0] != BasicType::NONE
    ) {

        throw TypeMismatch({}, "Optional must be Enum with 2 elements. Type of first element must be None");
    }

    return typeKey.typeParameters[1];
}

TypeCreator & typeCreator()
{
    static TypeCreator ret;
    return ret;
}
