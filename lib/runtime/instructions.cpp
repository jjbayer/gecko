#include "instructions.hpp"
#include "common/function.hpp"

namespace instructions {

SetInt::SetInt(ObjectId target, int64_t value)
    : mTarget(target)
    , mValue(value)
{

}

void SetInt::call(std::vector<Object> &data, InstructionPointer &ip) const
{
    data[mTarget].as_int = mValue;
}

AddInt::AddInt(ObjectId left, ObjectId right, ObjectId target)
    : mLeft(left)
    , mRight(right)
    , mTarget(target)
{

}

void AddInt::call(std::vector<Object> &data, InstructionPointer &ip) const
{
    data[mTarget].as_int = data[mLeft].as_int + data[mRight].as_int;
}


IntGte::IntGte(ObjectId left, ObjectId right, ObjectId target)
    : mLeft(left)
    , mRight(right)
    , mTarget(target)
{
}

void IntGte::call(std::vector<Object> &data, InstructionPointer &ip) const
{
    data[mTarget].as_boolean = data[mLeft].as_int >= data[mRight].as_int;
}

JumpIf::JumpIf(ObjectId condition, InstructionPointer ipNew)
    : mCondition(condition)
    , mIpNew(ipNew)
{

}

void JumpIf::call(std::vector<Object> &data, InstructionPointer &ip) const
{
    if( data[mCondition].as_int ) ip = mIpNew - 1;
}

Jump::Jump(InstructionPointer ipNew)
    : mIpNew(ipNew)
{
}

void Jump::call(std::vector<Object> &data, InstructionPointer &ip) const
{
    ip = mIpNew - 1;
}

Copy::Copy(ObjectId source, ObjectId target)
    : mSource(source)
    , mTarget(target)
{
}

void Copy::call(std::vector<Object> &data, InstructionPointer &ip) const
{
    data[mTarget] = data[mSource];
}

IntLessThan::IntLessThan(ObjectId left, ObjectId right, ObjectId target)
    : mLeft(left)
    , mRight(right)
    , mTarget(target)
{
}

void IntLessThan::call(std::vector<Object> &data, InstructionPointer &ip) const
{
    data[mTarget].as_boolean = data[mLeft].as_int < data[mRight].as_int;
}

Negate::Negate(ObjectId source, ObjectId target)
    : mSource(source)
    , mTarget(target)
{
}

void Negate::call(std::vector<Object> &data, InstructionPointer &ip) const
{
    data[mTarget].as_boolean = ! data[mSource].as_boolean;
}

Noop::Noop() {}

void Noop::call(std::vector<Object> &data, InstructionPointer &ip) const
{
    // Nothing to do.
}

SetFloat::SetFloat(ObjectId target, double value)
    : mTarget(target)
    , mValue(value)
{
}

void SetFloat::call(std::vector<Object> &data, InstructionPointer &ip) const
{
    data[mTarget].as_float = mValue;
}

CallFunction::CallFunction(ObjectId functionId, ObjectId firstArg, ObjectId target)
    : mFunctionId(functionId)
    , mFirstArg(firstArg)
    , mTarget(target)
{
}

void CallFunction::call(std::vector<Object> &data, InstructionPointer &ip) const
{
    auto & func = (*data[mFunctionId].as_function_ptr);

    auto returnValue = func.call(&data[mFirstArg]);

    data[mTarget] = returnValue;
}

SetFunction::SetFunction(ObjectId target, obj::Function *func)
    : mTarget(target)
    , mFunc(func)
{

}

void SetFunction::call(std::vector<Object> &data, InstructionPointer &ip) const
{
    data[mTarget].as_function_ptr = mFunc; // FIXME: memory management
}

SetBoolean::SetBoolean(ObjectId target, bool value)
    : mTarget(target)
    , mValue(value)
{
}

void SetBoolean::call(std::vector<Object> &data, InstructionPointer &ip) const
{
    data[mTarget].as_boolean = mValue;
}

OrTest::OrTest(ObjectId left, ObjectId right, ObjectId target)
    : mLeft(left)
    , mRight(right)
    , mTarget(target)
{
}

void OrTest::call(std::vector<Object> &data, InstructionPointer &ip) const
{
    data[mTarget].as_boolean = data[mLeft].as_boolean || data[mRight].as_boolean;
}

AndTest::AndTest(ObjectId left, ObjectId right, ObjectId target)
    : mLeft(left)
    , mRight(right)
    , mTarget(target)
{
}

void AndTest::call(std::vector<Object> &data, InstructionPointer &ip) const
{
    data[mTarget].as_boolean = data[mLeft].as_boolean && data[mRight].as_boolean;
}

IntLTE::IntLTE(ObjectId left, ObjectId right, ObjectId target)
    : mLeft(left)
    , mRight(right)
    , mTarget(target)
{
}

void IntLTE::call(std::vector<Object> &data, InstructionPointer &ip) const
{
    data[mTarget].as_boolean = data[mLeft].as_int <= data[mRight].as_int;
}

IsEqual::IsEqual(ObjectId left, ObjectId right, ObjectId target)
    : mLeft(left)
    , mRight(right)
    , mTarget(target)
{
}

void IsEqual::call(std::vector<Object> &data, InstructionPointer &ip) const
{
    data[mTarget].as_boolean = data[mLeft].as_int == data[mRight].as_int;
}

IsNotEqual::IsNotEqual(ObjectId left, ObjectId right, ObjectId target)
    : mLeft(left)
    , mRight(right)
    , mTarget(target)
{
}

void IsNotEqual::call(std::vector<Object> &data, InstructionPointer &ip) const
{
    data[mTarget].as_boolean = data[mLeft].as_int != data[mRight].as_int;
}

IntGTE::IntGTE(ObjectId left, ObjectId right, ObjectId target)
    : mLeft(left)
    , mRight(right)
    , mTarget(target)
{
}

void IntGTE::call(std::vector<Object> &data, InstructionPointer &ip) const
{
    data[mTarget].as_boolean = data[mLeft].as_int >= data[mRight].as_int;
}

IntGreaterThan::IntGreaterThan(ObjectId left, ObjectId right, ObjectId target)
    : mLeft(left)
    , mRight(right)
    , mTarget(target)
{

}

void IntGreaterThan::call(std::vector<Object> &data, InstructionPointer &ip) const
{
    data[mTarget].as_boolean = data[mLeft].as_int > data[mRight].as_int;
}

} // namespace instructions
