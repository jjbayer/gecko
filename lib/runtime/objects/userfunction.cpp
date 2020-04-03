#include "userfunction.hpp"
#include <iostream> // for testing

namespace obj {

    Object UserFunction::call(Object * args)
    {
        std::cout << "User function called\n";

        return {};
    }

}
