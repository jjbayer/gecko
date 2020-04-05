#include "instructions.hpp"
#include "instructions.hpp"
#include <runtime/memorymanager.hpp>
#include "runtime/objects/string.hpp"
#include "runtime/objects/tuple.hpp"
#include "runtime/output.hpp"

#include <sstream>


namespace instructions {

SetInt::SetInt(ObjectId target, int64_t value)
    : mTarget(target)
    , mValue(value)
{

}

std::string SetInt::toString() const { return "SetInt target=" + std::to_string(mTarget) + " value=" + std::to_string(mValue); }

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

std::string AddInt::toString() const
{
    return "AddInt left=" + std::to_string(mLeft) + " right=" + std::to_string(mRight) + " target=" + std::to_string(mTarget);
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

std::string IntGte::toString() const { return "IntGte left=" + std::to_string(mLeft) + " right=" + std::to_string(mRight) + " target=" + std::to_string(mTarget); }

void IntGte::call(std::vector<Object> &data, InstructionPointer &ip) const
{
    data[mTarget].as_boolean = data[mLeft].as_int >= data[mRight].as_int;
}

JumpIf::JumpIf(ObjectId condition, InstructionPointer ipNew)
    : mCondition(condition)
    , mIpNew(ipNew)
{

}

std::string JumpIf::toString() const { return "JumpIf condition=" + std::to_string(mCondition) +  " ip=" + std::to_string(mIpNew); }

void JumpIf::call(std::vector<Object> &data, InstructionPointer &ip) const
{
    if( data[mCondition].as_boolean ) ip = mIpNew - 1;
}


JumpIfNot::JumpIfNot(ObjectId condition, InstructionPointer ipNew)
    : mCondition(condition)
    , mIpNew(ipNew)
{

}

std::string JumpIfNot::toString() const { return "JumpIfNot condition=" + std::to_string(mCondition) +  " ip=" + std::to_string(mIpNew); }

void JumpIfNot::call(std::vector<Object> &data, InstructionPointer &ip) const
{
    if( ! data[mCondition].as_boolean ) ip = mIpNew - 1;
}

Jump::Jump(InstructionPointer ipNew)
    : mIpNew(ipNew)
{
}

std::string Jump::toString() const { return "Jump " + std::to_string(mIpNew); }

void Jump::call(std::vector<Object> &data, InstructionPointer &ip) const
{
    ip = mIpNew - 1;
}

Copy::Copy(ObjectId source, ObjectId target)
    : mSource(source)
    , mTarget(target)
{
}

std::string Copy::toString() const { return "Copy source=" + std::to_string(mSource) + " target=" + std::to_string(mTarget); }

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

std::string IntLessThan::toString() const { return "IntLessThan left=" + std::to_string(mLeft) + " right=" + std::to_string(mRight) + " target=" + std::to_string(mTarget); }

void IntLessThan::call(std::vector<Object> &data, InstructionPointer &ip) const
{
    data[mTarget].as_boolean = data[mLeft].as_int < data[mRight].as_int;
}

Negate::Negate(ObjectId source, ObjectId target)
    : mSource(source)
    , mTarget(target)
{
}

std::string Negate::toString() const { return "Negate source=" + std::to_string(mSource) + " target=" + std::to_string(mTarget); }

void Negate::call(std::vector<Object> &data, InstructionPointer &ip) const
{
    data[mTarget].as_boolean = ! data[mSource].as_boolean;
}

Noop::Noop() {}

std::string Noop::toString() const { return "Noop"; }

void Noop::call(std::vector<Object> &data, InstructionPointer &ip) const
{
    // Nothing to do.
}

SetFloat::SetFloat(ObjectId target, double value)
    : mTarget(target)
    , mValue(value)
{
}

std::string SetFloat::toString() const { return "SetFloat target=" + std::to_string(mTarget) + " target=" + std::to_string(mValue); }

void SetFloat::call(std::vector<Object> &data, InstructionPointer &ip) const
{
    data[mTarget].as_float = mValue;
}


std::string SetAllocated::toString() const
{
    return "SetFunction target=" + std::to_string(mTarget);
}

void SetAllocated::call(std::vector<Object> &data, InstructionPointer &ip) const
{
    data[mTarget].as_ptr = memory().add(mCreator());
}

SetAllocated::~SetAllocated() {}

SetBoolean::SetBoolean(ObjectId target, bool value)
    : mTarget(target)
    , mValue(value)
{
}

std::string SetBoolean::toString() const { return "SetBoolean target=" + std::to_string(mTarget) + " value=" + std::to_string(mValue); }

void SetBoolean::call(std::vector<Object> &data, InstructionPointer &ip) const
{
    data[mTarget].as_boolean = mValue;
}

SetString::SetString(ObjectId target, const std::string & value)
    : mTarget(target)
    , mValue(value)
{
}

std::string SetString::toString() const { return "SetString target=" + std::to_string(mTarget) + " value=" + mValue; }

void SetString::call(std::vector<Object> &data, InstructionPointer &ip) const
{
    data[mTarget].as_ptr = memory().add(std::make_unique<obj::String>(mValue));
}

OrTest::OrTest(ObjectId left, ObjectId right, ObjectId target)
    : mLeft(left)
    , mRight(right)
    , mTarget(target)
{
}

std::string OrTest::toString() const { return "OrTest left=" +  std::to_string(mLeft) + " right=" + std::to_string(mRight) + " target=" + std::to_string(mTarget); }

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

std::string AndTest::toString() const { return "AndTest left=" +  std::to_string(mLeft) + " right=" + std::to_string(mRight) + " target=" + std::to_string(mTarget); }

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

std::string IntLTE::toString() const { return "IntLTE left=" +  std::to_string(mLeft) + " right=" + std::to_string(mRight) + " target=" + std::to_string(mTarget); }

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

std::string IsEqual::toString() const { return "IsEqual left=" +  std::to_string(mLeft) + " right=" + std::to_string(mRight) + " target=" + std::to_string(mTarget); }

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

std::string IsNotEqual::toString() const { return "IsNotEqual left=" +  std::to_string(mLeft) + " right=" + std::to_string(mRight) + " target=" + std::to_string(mTarget); }

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

std::string IntGTE::toString() const { return "IntGTE left=" +  std::to_string(mLeft) + " right=" + std::to_string(mRight) + " target=" + std::to_string(mTarget); }

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

std::string IntGreaterThan::toString() const { return "IntGreaterThan left=" +  std::to_string(mLeft) + " right=" + std::to_string(mRight) + " target=" + std::to_string(mTarget); }

void IntGreaterThan::call(std::vector<Object> &data, InstructionPointer &ip) const
{
    data[mTarget].as_boolean = data[mLeft].as_int > data[mRight].as_int;
}

CollectGarbage::CollectGarbage(std::vector<ObjectId> keepObjects)
    :mKeepObjects(std::move(keepObjects))
{

}

std::string CollectGarbage::toString() const
{
    std::stringstream ret;
    ret << "Free";
    for(auto id : mKeepObjects) ret << " " << id;

    return ret.str();
}

void CollectGarbage::call(std::vector<Object> &data, InstructionPointer &ip) const
{
    std::set<obj::Allocated*> toBeKept;
    for(auto id : mKeepObjects ) toBeKept.insert( data[id].as_ptr );
    memory().collectGarbage(toBeKept);
}


void PrintInt::call(std::vector<Object> &data, InstructionPointer &ip) const
{
    *(getOutput().stdout) << data[mSource].as_int << "\n";
}


void PrintString::call(std::vector<Object> &data, InstructionPointer &ip) const
{
    *(getOutput().stdout) << static_cast<obj::String*>(data[mSource].as_ptr)->value() << "\n";
}












} // namespace instructions
