#include "executor.hpp"
#include <iostream>

void run(const std::vector<Instruction> &instructions)
{
    std::vector<Object> data(256); // FIXME: let compiler decide data size

    int count = 0;
    for(auto ip = 0; ip < instructions.size(); ip++) {
//        std::cout << "IP=" << ip << std::endl;
        instructions.at(ip)(data, ip);

        if( count++ > 100) break;
    }
}
