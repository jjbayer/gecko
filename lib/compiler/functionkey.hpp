#pragma once
#include "typecreator.hpp"
#include "common/utils.hpp"

#include <optional>
#include <string>
#include <vector>


namespace ct {

class FunctionKey
{
public:
    /// Lookup value types
    FunctionKey(const std::string & name);

    /// Lookup function types
    FunctionKey(const std::string & name, const std::vector<Type> & typeParameters, const std::vector<Type> & argumentTypes);

    bool operator==(const FunctionKey & other) const;

    std::string toString() const;

    const std::string mName;
    const std::vector<Type> mTypeParameters;
    const std::vector<Type> mArgumentTypes;

};

} // namespace ct

