#pragma once
#include "function.hpp"

class PrintInt: public obj::Function
{
public:
    Object call(const std::vector<Object> &args) override;
};
