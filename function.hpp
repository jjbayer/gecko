#pragma once
#include "object.hpp"
#include <vector>


namespace obj {

    class Function
    {
    public:

        virtual ObjectType returnType() const = 0;
        virtual std::vector<ObjectType> argumentTypes() const = 0;

        virtual Object call(const std::vector<Object> & args) = 0;
    };

}// namespace obj
