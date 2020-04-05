#include "lookupkey.hpp"

FunctionKey::FunctionKey(const std::string &name)
    : mName(name)
    , mArgumentTypes({})
{

}

FunctionKey::FunctionKey(const std::string &name, const std::vector<Type> &argumentTypes)
    : mName(name)
    , mArgumentTypes(argumentTypes)
{

}

bool FunctionKey::operator==(const FunctionKey &other) const
{
    // Name must be the same
    if( mName != other.mName ) return false;

    if( mArgumentTypes.size() != other.mArgumentTypes.size() ) return false;
    for(size_t i = 0; i < mArgumentTypes.size(); i++) {
        if( mArgumentTypes[i] != other.mArgumentTypes[i] ) {

            return false;
        }
    }

    return true;
}


