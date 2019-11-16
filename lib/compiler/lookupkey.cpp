#include "lookupkey.hpp"
#include "common/utils.hpp"

LookupKey::LookupKey(const std::string &name)
    : mName(name)
    , mArgumentTypes({})
{

}

LookupKey::LookupKey(const std::string &name, const std::vector<ValueType> &argumentTypes)
    : mName(name)
    , mArgumentTypes(argumentTypes)
{

}

bool LookupKey::operator==(const LookupKey &other) const
{
    // Name must be the same
    if( mName != other.mName ) return false;

    // Either both are value types or both are function types
    if( isFunction() != other.isFunction() ) return false;

    if( isFunction() ) {
        const auto & argumentTypes = *mArgumentTypes;
        const auto & otherArgumentTypes = *other.mArgumentTypes;
        if( argumentTypes.size() != otherArgumentTypes.size() ) return false;
        for(size_t i = 0; i < argumentTypes.size(); i++) {
            if( argumentTypes[i] != otherArgumentTypes[i] ) {

                return false;
            }
        }
    }

    return true;
}
