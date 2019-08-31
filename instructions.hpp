#pragma once
#include <functional>
#include <iostream>
#include <vector>

union Object {
    int64_t as_int;  // TODO: what  if 32 bit arch?
    double as_float;
    // RefCounted<std::string> as_string; // TODO: own string type
};

using ObjectId = int;
using InstructionPointer = int;
using Instruction = std::function<void(std::vector<Object> & data, InstructionPointer & ip)>;


Instruction setInt(ObjectId target, int64_t value)
{
    return [=](std::vector<Object> & data, InstructionPointer & ip) {
        data[target].as_int = value;
    };
}

Instruction addInt(ObjectId left, ObjectId right, ObjectId target)
{
    return [=](std::vector<Object> & data, InstructionPointer & ip) {
        data[target].as_int = data[left].as_int + data[right].as_int;
    };
}

Instruction printInt(ObjectId id)
{
    return [=](std::vector<Object> & data, InstructionPointer & ip) {
        std::cout << "[int " << data[id].as_int << "]\n";
    };
}

Instruction intGte(ObjectId left, ObjectId right, ObjectId target)
{
    return [=](std::vector<Object> & data, InstructionPointer & ip) {
        data[target].as_int = data[left].as_int >= data[right].as_int;
    };
}

Instruction jumpIf(ObjectId condition, InstructionPointer ipNew)
{
    return [=](std::vector<Object> & data, InstructionPointer & ip) {
        if( data[condition].as_int ) ip = ipNew - 1;
    };
}

Instruction jump(InstructionPointer ipNew)
{
    return [=](std::vector<Object> &, InstructionPointer & ip) {
        ip = ipNew - 1;
    };
}
