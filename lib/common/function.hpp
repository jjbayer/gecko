#pragma once
#include "object.hpp"
#include "compiler/type.hpp"
#include <vector>


namespace obj {

    class Function
    {
    public:

        virtual Object call(Object * args) = 0;

        virtual std::vector<ValueType> argumentTypes() const = 0;

        virtual ValueType returnType() const = 0;

        virtual ~Function() {}
    };

}// namespace obj
