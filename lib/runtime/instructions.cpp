#include "instructions.hpp"
#include "common/function.hpp"


Instruction setInt(ObjectId target, int64_t value)
{
    return [=](std::vector<Object> & data, InstructionPointer & ip) {
        data[target].as_int = value;
    };
}

Instruction addInt(ObjectId left, ObjectId right, ObjectId target)
{
    return [=](std::vector<Object> & data, InstructionPointer & ip) {
//        std::cout << data[left].as_int << " + " << data[right].as_int << "\n";
        data[target].as_int = data[left].as_int + data[right].as_int;
    };
}


Instruction intGte(ObjectId left, ObjectId right, ObjectId target)
{
    return [=](std::vector<Object> & data, InstructionPointer & ip) {
        data[target].as_boolean = data[left].as_int >= data[right].as_int;
    };
}

Instruction jumpIf(ObjectId condition, InstructionPointer ipNew)
{
    return [=](std::vector<Object> & data, InstructionPointer & ip) {
//        std::cout << "Do we jump? " << data[condition].as_int << std::endl;
        if( data[condition].as_int ) ip = ipNew - 1;
    };
}

Instruction jump(InstructionPointer ipNew)
{
    return [=](std::vector<Object> &, InstructionPointer & ip) {
        ip = ipNew - 1;
    };
}

Instruction copy(ObjectId source, ObjectId target)
{
    return [=](std::vector<Object> & data, InstructionPointer & ip) {
        data[target] = data[source];
    };
}

Instruction intLessThan(ObjectId left, ObjectId right, ObjectId target)
{
    return [=](std::vector<Object> & data, InstructionPointer & ip) {
//        std::cout << data[left].as_int << " < " << data[right].as_int << std::endl;
        data[target].as_boolean = data[left].as_int < data[right].as_int;
    };
}

Instruction negateInt(ObjectId source, ObjectId target)
{
    return [=](std::vector<Object> & data, InstructionPointer & ip) {
        const auto value = data[source].as_boolean;
        data[target].as_boolean = value ? 0 : 1;
    };
}

Instruction noop()
{
    return [](std::vector<Object> & data, InstructionPointer & ip) {};
}

Instruction setFloat(ObjectId target, double value)
{
    return [=](std::vector<Object> & data, InstructionPointer & ip) {
        data[target].as_float = value;
    };
}

Instruction callFunction(ObjectId functionId, ObjectId firstArg, ObjectId target)
{
    return [=](std::vector<Object> & data, InstructionPointer & ip) {
        auto & func = (*data[functionId].as_function_ptr);

        auto returnValue = func.call(&data[firstArg]);

        data[target] = returnValue;
    };
}

Instruction setFunction(ObjectId target, obj::Function *func)
{
    return [=](std::vector<Object> & data, InstructionPointer & ip) {

        data[target].as_function_ptr = func; // FIXME: memory management
    };
}

Instruction setBoolean(ObjectId target, bool value)
{
    return [=](std::vector<Object> & data, InstructionPointer & ip) {
        data[target].as_boolean = value;
    };
}
