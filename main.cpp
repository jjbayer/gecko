#include <vector>
#include <iostream>
#include <functional>


// template<typename T>
// struct RefCountedData
// {
//     int refCount = 0;
//     T value;
// }

// template<typename T>
// struct RefCounted
// {
//     // TODO: prevent copies
//     RefCounted(T value) { data = new RefCountedData<T> {1, value}; }

//     RefCounted(const RefCounted<T> & other)
//     {
//         data = other.data;
//         data->refCount++;
//     }

//     ~RefCounted()
//     {
//         data->refCount--;
//         std::cout << "RefCount down to " << data->refCount;

//         if( data->refCount == 0 ) {
//             delete data;
//         }
//     }

//     RefCountedData<T> * data = nullptr;
// }


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



int main()
{



    std::vector<Instruction> instructions {
        setInt(0, 666),
        setInt(1, 123),
        addInt(0, 1, 2),
        printInt(2),
        setInt(3, 0),
        setInt(4, 10),
        intGte(3, 4, 5),
        jumpIf(5, 12),
            printInt(3),
            setInt(6, 1),
            addInt(3, 6, 3),
        jump(6),
        setInt(12, 1337),
        printInt(12),
    };

    std::vector<Object> data(100);
    for(auto i = 0; i < instructions.size(); i++) {
        instructions[i](data, i);
    }

    return 0;
}
