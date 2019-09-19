#pragma once
#include "function.hpp"

class PrintInt: public obj::Function
{
public:
    ObjectType returnType() const override;
    std::vector<ObjectType> argumentTypes() const override;

    Object call(const std::vector<Object> &args) override;
};


class AddInt: public obj::Function
{
public:
    ObjectType returnType() const override;
    std::vector<ObjectType> argumentTypes() const override;

    Object call(const std::vector<Object> &args) override;
};
