#pragma once
#include "instructions.hpp"


void run(const std::vector<Instruction> & instructions)
{
    std::vector<Object> data(256); // FIXME: let compiler decide data size

    InstructionPointer ip = 0;
    for(auto & instruction : instructions) {
        instruction(data, ip);
        ip++;
    }
}
