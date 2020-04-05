#include "stdin.hpp"
#include "runtime/instructions.hpp"


namespace ct {


void NextStdin::_generateInstructions(
    const std::vector<std::shared_ptr<const CompileTimeObject> > & arguments,
    std::vector<std::unique_ptr<Instruction> > & instructions,
    std::shared_ptr<CompileTimeObject> returnValue
) const
{
    // Prepare output variable
    const TypeKey typeKey {MetaType::ENUM, {BasicType::NONE, BasicType::STRING}};
    returnValue->type = typeCreator().getType(typeKey);
    instructions.push_back(
        std::make_unique<instructions::SetAllocated>(
            returnValue->id, &std::make_unique<obj::Tuple<2> >
        )
    );

    // TODO: actually read from given object
    instructions.push_back(std::make_unique<instructions::ReadFromStdin>(returnValue->id));
}

std::vector<Type> NextStdin::argumentTypes() const { return {typeCreator().structType("Stdin")}; }


// Type NextStdin::returnType() const
// {
//     const TypeKey typeKey {MetaType::ENUM, {BasicType::NONE, BasicType::STRING}};
//     return typeCreator().getType(typeKey);
// }


// Object NextStdin::call(Object *args)
// {
//     Object ret;

//     auto tuple = std::make_unique<obj::Tuple<2>>();
//     if(  std::cin.eof() ) {
//         tuple->data[0].as_int = 0;
//     } else {

//         std::string value;
//         std::getline(std::cin, value);

//         // FIXME: check for errors
//         tuple->data[0].as_int = 1;
//         auto ptr = std::make_unique<obj::String>(value);
//         tuple->data[1].as_ptr = memory().add(std::move(ptr));
//     }

//     ret.as_ptr = memory().add(std::move(tuple));

//     return ret;
// }

// std::vector<Type> NextStdin::argumentTypes() const
// {
//     return { typeCreator().structType("Stdin") };
// }


} // namespace obj
