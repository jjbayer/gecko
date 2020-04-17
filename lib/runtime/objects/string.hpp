#pragma once
#include "allocated.hpp"
#include <string>


namespace obj {

// TODO: single pointer
class String: public Childless
{
public:
    String(std::string value);

    const std::string & value() const { return mValue; }

private:
    std::string mValue;
};

} // namespace obj
