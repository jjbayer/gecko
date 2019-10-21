#include "executor.hpp"
#include <iostream>

void run(const std::vector<std::unique_ptr<Instruction> > &instructions, int numObjects)
{
    std::vector<Object> data(numObjects);

    for(size_t ip = 0; ip < instructions.size(); ip++) {
//        std::cout << "IP=" << ip << std::endl;
        auto & instruction = instructions.at(ip);
        instruction->call(data, ip);
    }
}
