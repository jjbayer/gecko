#include "builtins.hpp"
#include "runtime/objects/string.hpp"
#include "runtime/objects/tuple.hpp"
#include "runtime/memorymanager.hpp"
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

Object NextStdin::call(Object *args)
{
    Object ret;

    auto tuple = std::make_unique<obj::Tuple<2>>();
    if(  std::cin.eof() ) {
        tuple->data[0].as_int = 0;
    } else {

        std::string value;
        std::getline(std::cin, value);

        // FIXME: check for errors
        tuple->data[0].as_int = 1;
        auto ptr = std::make_unique<obj::String>(value);
        tuple->data[1].as_ptr = memory().add(std::move(ptr));
    }

    ret.as_ptr = memory().add(std::move(tuple));

    return ret;
}

std::vector<Type> NextStdin::argumentTypes() const
{
    return { typeCreator().structType("Stdin") };
}
