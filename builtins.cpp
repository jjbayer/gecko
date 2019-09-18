#include "builtins.hpp"
#include <iostream>

Object PrintInt::call(const std::vector<Object> &args)
{
    std::cout << "[int " << args[0].as_int << "]\n";

    Object ret;

    ret.as_int = 0; // TODO: NoneType

    return ret;
}
