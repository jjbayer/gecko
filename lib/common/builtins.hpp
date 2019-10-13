#pragma once
#include "function.hpp"

class PrintInt: public obj::Function
{
public:
    ObjectType returnType() const override;
    std::vector<ObjectType> argumentTypes() const override;

    Object call(Object * args) override;
};


class AddInt: public obj::Function
{
public:
    ObjectType returnType() const override;
    std::vector<ObjectType> argumentTypes() const override;

    Object call(Object * args) override;
};


class Dummy: public obj::Function
{
public:
    ObjectType returnType() const override { return ObjectType::INT; }
    std::vector<ObjectType> argumentTypes() const override { return {}; }

    Object call(Object * args) override;
};