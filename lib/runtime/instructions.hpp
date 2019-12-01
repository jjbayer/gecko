#pragma once
#include "common/object.hpp"
#include "runtime/objects/tuple.hpp"

#include <functional>
#include <memory>
#include <ostream>
#include <sstream>
#include <vector>


namespace obj { class Function; }


using InstructionPointer = size_t;


class Instruction
{
public:
    virtual std::string toString() const = 0;
    /// call() can be const because it does not alter internal state of instruction
    virtual void call(std::vector<Object> & data, InstructionPointer & ip) const = 0;
    virtual ~Instruction() {}
};


namespace instructions {


// TODO: set literals only once
class SetInt: public Instruction
{
public:
    SetInt(ObjectId target, int64_t value);
    std::string toString() const override;
    void call(std::vector<Object> & data, InstructionPointer & ip) const override;

    ~SetInt() override {}

private:
    const ObjectId mTarget;
    const int64_t mValue;
};


class AddInt: public Instruction
{
public:
    AddInt(ObjectId left, ObjectId right, ObjectId target);
    std::string toString() const override;
    void call(std::vector<Object> & data, InstructionPointer & ip) const override;
    ~AddInt() override {}

private:
    const ObjectId mLeft;
    const ObjectId mRight;
    const ObjectId mTarget;
};


class SetFloat: public Instruction
{
public:
    SetFloat(ObjectId target, double value);
    std::string toString() const override;
    void call(std::vector<Object> & data, InstructionPointer & ip) const override;
    ~SetFloat() override {}

private:
    const ObjectId mTarget;
    const double mValue;
};


class SetBoolean: public Instruction
{
public:
    SetBoolean(ObjectId target, bool value);
    std::string toString() const override;
    void call(std::vector<Object> & data, InstructionPointer & ip) const override;
    ~SetBoolean() override {}

private:
    const ObjectId mTarget;
    const bool mValue;
};


class SetString: public Instruction
{
public:
    SetString(ObjectId target, const std::string & value);
    std::string toString() const override;
    void call(std::vector<Object> & data, InstructionPointer & ip) const override;
    ~SetString() override {}

private:
    const ObjectId mTarget;
    const std::string mValue;
};


class SetAllocated: public Instruction
{
public:
    template<typename Fn>
    SetAllocated(ObjectId target, Fn creator)
        : mTarget(target)
        , mCreator(creator)
    {

    }
    std::string toString() const override;
    void call(std::vector<Object> & data, InstructionPointer & ip) const override;
    ~SetAllocated() override;

private:
    const ObjectId mTarget;
    std::function<std::unique_ptr<obj::Allocated>()> mCreator;
};


class IntGte: public Instruction
{
public:
    IntGte(ObjectId left, ObjectId right, ObjectId target);
    std::string toString() const override;
    void call(std::vector<Object> & data, InstructionPointer & ip) const override;
    ~IntGte() override {}

private:
    const ObjectId mLeft;
    const ObjectId mRight;
    const ObjectId mTarget;
};


// TODO: replace JumpIf with JumpIfNot everywhere
class JumpIf: public Instruction
{
public:
    JumpIf(ObjectId condition, InstructionPointer ipNew);
    std::string toString() const override;
    void call(std::vector<Object> & data, InstructionPointer & ip) const override;
    ~JumpIf() override {}

private:
    const ObjectId mCondition;
    const InstructionPointer mIpNew;
};


class JumpIfNot: public Instruction
{
public:
    JumpIfNot(ObjectId condition, InstructionPointer ipNew);
    std::string toString() const override;
    void call(std::vector<Object> & data, InstructionPointer & ip) const override;
    ~JumpIfNot() override {}

private:
    const ObjectId mCondition;
    const InstructionPointer mIpNew;
};


class Jump: public Instruction
{
public:
    Jump(InstructionPointer ipNew);
    std::string toString() const override;
    void call(std::vector<Object> & data, InstructionPointer & ip) const override;
    ~Jump() override {}

private:
    const InstructionPointer mIpNew;
};


class Copy: public Instruction
{
public:
    Copy(ObjectId source, ObjectId target);
    std::string toString() const override;
    void call(std::vector<Object> & data, InstructionPointer & ip) const override;
    ~Copy() override {}

private:
    const ObjectId mSource;
    const ObjectId mTarget;
};


class IntLessThan: public Instruction
{
public:
    IntLessThan(ObjectId left, ObjectId right, ObjectId target);
    std::string toString() const override;
    void call(std::vector<Object> & data, InstructionPointer & ip) const override;
    ~IntLessThan() override {}

private:
    const ObjectId mLeft;
    const ObjectId mRight;
    const ObjectId mTarget;
};


class IntLTE: public Instruction
{
public:
    IntLTE(ObjectId left, ObjectId right, ObjectId target);
    std::string toString() const override;
    void call(std::vector<Object> & data, InstructionPointer & ip) const override;
    ~IntLTE() override {}

private:
    const ObjectId mLeft;
    const ObjectId mRight;
    const ObjectId mTarget;
};


class IsEqual: public Instruction
{
public:
    IsEqual(ObjectId left, ObjectId right, ObjectId target);
    std::string toString() const override;
    void call(std::vector<Object> & data, InstructionPointer & ip) const override;
    ~IsEqual() override {}

private:
    const ObjectId mLeft;
    const ObjectId mRight;
    const ObjectId mTarget;
};


class IsNotEqual: public Instruction
{
public:
    IsNotEqual(ObjectId left, ObjectId right, ObjectId target);
    std::string toString() const override;
    void call(std::vector<Object> & data, InstructionPointer & ip) const override;
    ~IsNotEqual() override {}

private:
    const ObjectId mLeft;
    const ObjectId mRight;
    const ObjectId mTarget;
};


class IntGTE: public Instruction
{
public:
    IntGTE(ObjectId left, ObjectId right, ObjectId target);
    std::string toString() const override;
    void call(std::vector<Object> & data, InstructionPointer & ip) const override;
    ~IntGTE() override {}

private:
    const ObjectId mLeft;
    const ObjectId mRight;
    const ObjectId mTarget;
};


class IntGreaterThan: public Instruction
{
public:
    IntGreaterThan(ObjectId left, ObjectId right, ObjectId target);
    std::string toString() const override;
    void call(std::vector<Object> & data, InstructionPointer & ip) const override;
    ~IntGreaterThan() override {}

private:
    const ObjectId mLeft;
    const ObjectId mRight;
    const ObjectId mTarget;
};



class OrTest: public Instruction
{
public:
    OrTest(ObjectId left, ObjectId right, ObjectId target);
    std::string toString() const override;
    void call(std::vector<Object> & data, InstructionPointer & ip) const override;
    ~OrTest() override {}

private:
    const ObjectId mLeft;
    const ObjectId mRight;
    const ObjectId mTarget;
};


class AndTest: public Instruction
{
public:
    AndTest(ObjectId left, ObjectId right, ObjectId target);
    std::string toString() const override;
    void call(std::vector<Object> & data, InstructionPointer & ip) const override;
    ~AndTest() override {}

private:
    const ObjectId mLeft;
    const ObjectId mRight;
    const ObjectId mTarget;
};


class Negate: public Instruction
{
public:
    Negate(ObjectId source, ObjectId target);
    std::string toString() const override;
    void call(std::vector<Object> & data, InstructionPointer & ip) const override;
    ~Negate() override {}

private:
    const ObjectId mSource;
    const ObjectId mTarget;
};


/// Return value will be written to target
class CallFunction: public Instruction
{
public:
    CallFunction(ObjectId functionId, ObjectId firstArg, ObjectId target);
    std::string toString() const override;
    void call(std::vector<Object> & data, InstructionPointer & ip) const override;
    ~CallFunction() override {}

private:
    const ObjectId mFunctionId;
    const ObjectId mFirstArg;
    const ObjectId mTarget;
};


class Noop: public Instruction
{
public:
    Noop();
    std::string toString() const override;
    void call(std::vector<Object> & data, InstructionPointer & ip) const override;
    ~Noop() override {}
};


class CollectGarbage: public Instruction
{
public:
    CollectGarbage(std::vector<ObjectId> keepObjects);
    std::string toString() const override;
    void call(std::vector<Object> & data, InstructionPointer & ip) const override;
    ~CollectGarbage() override {}

private:
    const std::vector<ObjectId> mKeepObjects;
};


template<int Index, int TupleSize>
class ReadFromTuple: public Instruction
{
public:

    ReadFromTuple(ObjectId tuple, ObjectId target)
        : mTuple(tuple)
        , mTarget(target)
    {

    }
    std::string toString() const override
    {
        std::stringstream stream;
        stream << "ReadFromTuple " << mTuple << " " << Index << " " <<  mTarget;

        return stream.str();
    }
    void call(std::vector<Object> & data, InstructionPointer & ip) const override
    {
        auto tuple = static_cast<obj::Tuple<TupleSize> *>(data[mTuple].as_ptr);
        data[mTarget] = std::get<Index>(tuple->data);
    }

private:
    const ObjectId mTuple;
    const ObjectId mTarget;
};


template<int Index, int TupleSize>
class WriteToTuple: public Instruction
{
public:

    WriteToTuple(ObjectId tuple, ObjectId source)
        : mTuple(tuple)
        , mSource(source)
    {

    }

    std::string toString() const override
    {
        std::stringstream stream;
        stream << "WriteToTuple " << mTuple << " " << mIndex << " " <<  mSource;

        return stream.str();
    }

    void call(std::vector<Object> & data, InstructionPointer & ip) const override
    {
        auto tuple = static_cast<obj::Tuple<TupleSize> *>(data[mTuple].as_ptr);
        std::get<Index>(tuple->data[mIndex]) = data[mSource];
    }

private:
    const ObjectId mTuple;
    const size_t mIndex;
    const ObjectId mSource;
};

} // namespace instructions
