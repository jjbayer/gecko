#pragma once
#include "function.hpp"

class PrintInt: public obj::Function
{
public:
    Object call(Object * args) override;

    std::vector<ValueType> argumentTypes() const
    {
        return { ValueType::INT };
    }

    ValueType returnType() const { return ValueType::INT; }
};


class AddInt: public obj::Function
{
public:
    Object call(Object * args) override;

    std::vector<ValueType> argumentTypes() const
    {
        return { ValueType::INT, ValueType::INT };
    }

    ValueType returnType() const { return ValueType::INT; }

};


class Dummy: public obj::Function
{
public:
    Object call(Object * args) override;

    std::vector<ValueType> argumentTypes() const { return {}; }
    ValueType returnType() const{ return ValueType::INT; }
};
