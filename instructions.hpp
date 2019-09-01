#pragma once
#include <functional>
#include <vector>


union Object {
    int64_t as_int;  // TODO: what  if 32 bit arch?
    double as_float;
    // RefCounted<std::string> as_string; // TODO: own string type
};

using ObjectId = int;
using InstructionPointer = int;
using Instruction = std::function<void(std::vector<Object> & data, InstructionPointer & ip)>;


Instruction setInt(ObjectId target, int64_t value);

Instruction addInt(ObjectId left, ObjectId right, ObjectId target);

Instruction printInt(ObjectId id);

Instruction intGte(ObjectId left, ObjectId right, ObjectId target);

Instruction jumpIf(ObjectId condition, InstructionPointer ipNew);

Instruction jump(InstructionPointer ipNew);

Instruction copy(ObjectId source, ObjectId target);
