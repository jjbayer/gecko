#pragma once
#include "object.hpp"

#include <functional>
#include <vector>


using InstructionPointer = int;
using Instruction = std::function<void(std::vector<Object> & data, InstructionPointer & ip)>;


Instruction setInt(ObjectId target, int64_t value);

Instruction addInt(ObjectId left, ObjectId right, ObjectId target);

Instruction setFloat(ObjectId target, double value);

Instruction setFunction(ObjectId target, obj::Function * func);

Instruction intGte(ObjectId left, ObjectId right, ObjectId target);

Instruction jumpIf(ObjectId condition, InstructionPointer ipNew);

Instruction jump(InstructionPointer ipNew);

Instruction copy(ObjectId source, ObjectId target);

Instruction intLessThan(ObjectId left, ObjectId right, ObjectId target);

Instruction negateInt(ObjectId source, ObjectId target);

/// Return value will be written to target
Instruction callFunction(ObjectId fn, std::vector<ObjectId> argIds, ObjectId target);

Instruction noop();
