#include "builtins.hpp"
#include <iostream>


Object PrintInt::call(Object *args)
{
    std::cout << "[int " << args[0].as_int << "]\n";

    Object ret;

    ret.as_int = 0; // TODO: NoneType

    return ret;
}


Object AddInt::call(Object *args)
{
    Object ret;
    ret.as_int = args[0].as_int + args[1].as_int;

    return ret;
}

Object Dummy::call(Object *args)
{
    //  TODO: make void, target object as output argument
    Object ret;
    ret.as_int = 5;
    return ret;
}
