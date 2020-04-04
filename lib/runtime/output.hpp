#pragma once
#include <iostream>


struct Output
{
    std::ostream * stdout = &std::cout;
};


Output & getOutput();








