#pragma once
#include "allocated.hpp"
#include "common/object.hpp"
#include "compiler/typecreator.hpp"
#include <vector>


namespace obj {

    class Function: public Allocated
    {
    public:

        virtual Object call(Object * args) = 0;

        virtual std::vector<Type> argumentTypes() const = 0;

        virtual Type returnType() const = 0;

        virtual ~Function() {}
    };

}// namespace obj
