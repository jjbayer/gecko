#include <vector>
#include <iostream>


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


struct Object
{
    enum Type
    {
        INT,
        FLOAT,
        STRING
    };

    Type type;
    union {
        int64_t as_int;  // TODO: what  if 32 bit arch?
        double as_float;
        // RefCounted<std::string> as_string; // TODO: own string type
    };
};



enum InstructionType
{
    SET_INT,
    ADD_INT,
    INT_GTE,
    JUMP,
    JUMP_IF,
    PRINT_INT,
};

struct Instruction
{
    InstructionType type;
    int arg0 = 0;
    int arg1 = 0;
    int arg2 = 0; // TODO: args as union. same union as object?
};

void execute(const std::vector<Instruction> & instructions, std::vector<Object> & data)
{
    const auto maxCount = 200;
    int count = 0;

    for(auto i = 0; i < instructions.size(); i++) {
        // std::cout << "Count = " << count++ <<  ", IP = " << i << std::endl;
        const auto & instruction = instructions[i];
        switch(instruction.type) {
            case SET_INT:
                data[instruction.arg0].type = Object::INT;
                data[instruction.arg0].as_int = instruction.arg1;
                break;
            case ADD_INT:
                data[instruction.arg2].as_int = data[instruction.arg0].as_int + data[instruction.arg1].as_int;
                break;
            case INT_GTE:
                data[instruction.arg2].as_int = data[instruction.arg0].as_int >= data[instruction.arg1].as_int;
                break;
            case JUMP:
                i = instruction.arg0 - 1; // Because of i++, slightly stupid
                // std::cout << "IP after jump should be " << i << std::endl;
                break;
            case JUMP_IF:
                if( data[instruction.arg0].as_int ) {
                    i = instruction.arg1 - 1; // Because of i++, slightly stupid
                }
                break;
            case PRINT_INT:
                std::cout << data[instruction.arg0].as_int << std::endl;
                break;
            default:
                std::cerr << "Not implemented: " << instruction.type;
                break;
        }

        if( count > maxCount ) {
            return;
        }
    }
}




















int main()
{
    std::vector<Instruction> instructions {
        { SET_INT, 0, 666 },
        { SET_INT, 1, 123 },
        { ADD_INT, 0, 1, 2 },
        { PRINT_INT, 2 },
        { SET_INT, 3, 0 },
        { SET_INT, 4, 10 },
        { INT_GTE, 3, 4, 5 },
        { JUMP_IF, 5, 12 },
            { PRINT_INT, 3 },
            { SET_INT, 6, 1 },
            { ADD_INT, 3, 6, 3 },
        { JUMP, 6 },
        { SET_INT, 12, 1337},
        { PRINT_INT, 12 },
    };

    std::vector<Object> data(100);

    execute(instructions, data);


    return 0;
}
