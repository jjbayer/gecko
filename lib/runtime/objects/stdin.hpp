#pragma once
#include "function.hpp"


namespace obj {


class NextStdin: public Function
{
public:
    Object call(Object * args) override;

    std::vector<Type> argumentTypes() const;
    Type returnType() const;
};

} // namespace obj

