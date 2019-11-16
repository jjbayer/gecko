#pragma once
#include "function.hpp"

class PrintInt: public obj::Function
{
public:
    Object call(Object * args) override;

    std::vector<Type> argumentTypes() const
    {
        return { BasicType::INT };
    }

    Type returnType() const { return BasicType::INT; }
};


class AddInt: public obj::Function
{
public:
    Object call(Object * args) override;

    std::vector<Type> argumentTypes() const
    {
        return { BasicType::INT, BasicType::INT };
    }

    Type returnType() const { return BasicType::INT; }

};


class Dummy: public obj::Function
{
public:
    Object call(Object * args) override;

    std::vector<Type> argumentTypes() const { return {}; }
    Type returnType() const{ return BasicType::INT; }
};
