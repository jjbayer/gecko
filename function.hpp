#pragma once
#include "object.hpp"
#include <vector>


namespace obj {

    class Function
    {
    public:
        virtual Object call(const std::vector<Object> & args) = 0;
    };

}// namespace obj
