#pragma once
#include "allocated.hpp"
#include <string>


namespace obj {


class String: public Allocated
{
public:
    String(std::string value);

private:
    std::string mValue;
};

} // namespace obj
