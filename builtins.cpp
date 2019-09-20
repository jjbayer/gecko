#include "builtins.hpp"
#include <iostream>

ObjectType PrintInt::returnType() const
{
    return ObjectType::INT; // TODO: object type void / none
}

std::vector<ObjectType> PrintInt::argumentTypes() const
{
    return {ObjectType::INT};
}

Object PrintInt::call(Object *args)
{
    std::cout << "[int " << args[0].as_int << "]\n";

    Object ret;

    ret.as_int = 0; // TODO: NoneType

    return ret;
}

ObjectType AddInt::returnType() const
{
    return ObjectType::INT;
}

std::vector<ObjectType> AddInt::argumentTypes() const
{
    return {ObjectType::INT, ObjectType::INT};
}

Object AddInt::call(Object *args)
{
    Object ret;
    ret.as_int = args[0].as_int + args[1].as_int;

    return ret;
}
