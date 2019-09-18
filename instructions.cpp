#include "instructions.hpp"
#include "function.hpp"


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
        data[target].as_bool = data[left].as_int >= data[right].as_int;
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
        data[target].as_bool = data[left].as_int < data[right].as_int;
    };
}

Instruction negateInt(ObjectId source, ObjectId target)
{
    return [=](std::vector<Object> & data, InstructionPointer & ip) {
        const auto value = data[source].as_bool;
        data[target].as_bool = value ? 0 : 1;
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

Instruction callFunction(ObjectId fn, std::vector<ObjectId> argIds, ObjectId target)
{
    return [=](std::vector<Object> & data, InstructionPointer & ip) {
        std::vector<Object> args;
        for(auto argId : argIds) args.push_back(data[argId]);
        auto & func = (*data[fn].as_function_ptr);

        data[target] = func.call(args);
    };
}

Instruction setFunction(ObjectId target, obj::Function *func)
{
    return [=](std::vector<Object> & data, InstructionPointer & ip) {

        data[target].as_function_ptr = func; // FIXME: memory management
    };
}
