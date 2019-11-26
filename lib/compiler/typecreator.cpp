#include "typecreator.hpp"
#include "common/exceptions.hpp"


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
        mReverse[mNextType] = key;
        mNextType++;
    }

    return it->second;
}

const TypeKey &TypeCreator::getTypeKey(Type type) const
{
    return mReverse.at(type);
}

Type getOptionalType(const TypeCreator &typeCreator, Type type)
{
    const auto & typeKey = typeCreator.getTypeKey(type);
    if( typeKey.metaType != MetaType::ENUM
            || typeKey.arguments.size() != 2
            || typeKey.arguments[0] != BasicType::NONE
    ) {

        throw TypeMismatch({}, "Optional must be Enum with 2 elements. Type of first element must be None");
    }

    return typeKey.arguments[1];
}
