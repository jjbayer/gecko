#pragma once
#include "common/object.hpp"

#include <functional>
#include <vector>


using InstructionPointer = int;
using Instruction = std::function<void(std::vector<Object> & data, InstructionPointer & ip)>;


Instruction setInt(ObjectId target, int64_t value);

Instruction addInt(ObjectId left, ObjectId right, ObjectId target);

Instruction setFloat(ObjectId target, double value);

Instruction setBoolean(ObjectId target, bool value);

Instruction setFunction(ObjectId target, obj::Function * func);

Instruction intGte(ObjectId left, ObjectId right, ObjectId target);

Instruction jumpIf(ObjectId condition, InstructionPointer ipNew);

Instruction jump(InstructionPointer ipNew);

Instruction copy(ObjectId source, ObjectId target);

Instruction intLessThan(ObjectId left, ObjectId right, ObjectId target);

Instruction intLTE(ObjectId left, ObjectId right, ObjectId target);

Instruction isEqual(ObjectId left, ObjectId right, ObjectId target);

Instruction isNotEqual(ObjectId left, ObjectId right, ObjectId target);

Instruction intGTE(ObjectId left, ObjectId right, ObjectId target);

Instruction intGreaterThan(ObjectId left, ObjectId right, ObjectId target);


Instruction orTest(ObjectId left, ObjectId right, ObjectId target);

Instruction andTest(ObjectId left, ObjectId right, ObjectId target);

Instruction negate(ObjectId source, ObjectId target);

/// Return value will be written to target
Instruction callFunction(ObjectId functionId, ObjectId firstArg, ObjectId target);

Instruction noop();

