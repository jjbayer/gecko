#pragma once
#include "instructions.hpp"

#include <memory>
#include <vector>

void run(const std::vector<std::unique_ptr<Instruction> > &instructions, int numObjectIds);
