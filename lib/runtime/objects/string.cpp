#include "string.hpp"
#include <string>


namespace obj {

String::String(std::string value)
    : mValue(std::move(value))
{

}



} // namespace obj
