#pragma once
#include "object.hpp"
#include "compiler/typecreator.hpp"
#include <vector>


namespace obj {

    class Function
    {
    public:

        virtual Object call(Object * args) = 0;

        virtual std::vector<Type> argumentTypes() const = 0;

        virtual Type returnType() const = 0;

        virtual ~Function() {}
    };

}// namespace obj
