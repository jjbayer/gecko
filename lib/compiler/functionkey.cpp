#include "functionkey.hpp"


namespace ct {

FunctionKey::FunctionKey(const std::string &name, const std::vector<Type> &typeParameters, const std::vector<Type> &argumentTypes)
    : mName(name)
    , mTypeParameters(typeParameters)
    , mArgumentTypes(argumentTypes)
{

}

bool FunctionKey::operator==(const FunctionKey &other) const
{
    // Name must be the same
    if( mName != other.mName ) return false;

    if( mTypeParameters.size() != other.mTypeParameters.size() ) return false;
    for(size_t i = 0; i < mTypeParameters.size(); i++) {
        if( mTypeParameters[i] != other.mTypeParameters[i] ) {

            return false;
        }
    }

    if( mArgumentTypes.size() != other.mArgumentTypes.size() ) return false;
    for(size_t i = 0; i < mArgumentTypes.size(); i++) {
        if( mArgumentTypes[i] != other.mArgumentTypes[i] ) {

            return false;
        }
    }

    return true;
}

std::string FunctionKey::toString() const {
    auto ret = mName;
    if( ! mTypeParameters.empty() ) {
        ret += "<";
        auto tail = false;
        for(auto type : mTypeParameters) {
            if(tail) ret += ", ";
            ret += typeCreator().getTypeKey(type).toString();
            tail = true;
        }
        ret += ">";
    }
    ret += "(";
    auto tail = false;
    for(auto type : mArgumentTypes) {
        if(tail) ret += ", ";
        ret += typeCreator().getTypeKey(type).toString();
        tail = true;
    }
    ret += ")";

    return ret;
}

} // namespace ct

