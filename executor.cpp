#include "executor.hpp"
#include <iostream>

void run(const std::vector<Instruction> &instructions, int numObjects)
{
    std::vector<Object> data(numObjects);

    for(auto ip = 0; ip < instructions.size(); ip++) {
//        std::cout << "IP=" << ip << std::endl;
        instructions.at(ip)(data, ip);
    }
}
