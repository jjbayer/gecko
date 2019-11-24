#include "builtins.hpp"
#include "runtime/objects/string.hpp"
#include <iostream>


Object PrintInt::call(Object *args)
{
    std::cout << "[int " << args[0].as_int << "]\n";

    return {};
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

Object PrintString::call(Object *args)
{
    const auto * string = static_cast<obj::String*>(args[0].as_ptr);
    std::cout << string->value() << "\n";

    return {};
}
