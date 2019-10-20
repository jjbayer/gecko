#pragma once
#include "common/object.hpp"

#include <vector>


using InstructionPointer = int;


class Instruction
{
public:
    virtual const char * name() const = 0;
    /// call() can be const because it does not alter internal state of instruction
    virtual void call(std::vector<Object> & data, InstructionPointer & ip) const = 0;
    virtual ~Instruction() {}
};


namespace instructions {


class SetInt: public Instruction
{
public:
    SetInt(ObjectId target, int64_t value);
    const char * name() const override { return "SetInt"; }
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
    const char * name() const override { return "AddInt"; }
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
    const char * name() const override { return "SetFloat"; }
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
    const char * name() const override { return "SetBoolean"; }
    void call(std::vector<Object> & data, InstructionPointer & ip) const override;
    ~SetBoolean() override {}

private:
    const ObjectId mTarget;
    const bool mValue;
};


class SetFunction: public Instruction
{
public:
    SetFunction(ObjectId target, obj::Function * func);
    const char * name() const override { return "SetFunction"; }
    void call(std::vector<Object> & data, InstructionPointer & ip) const override;
    ~SetFunction() override {}

private:
    const ObjectId mTarget;
    obj::Function * const mFunc;
};


class IntGte: public Instruction
{
public:
    IntGte(ObjectId left, ObjectId right, ObjectId target);
    const char * name() const override { return "IntGte"; }
    void call(std::vector<Object> & data, InstructionPointer & ip) const override;
    ~IntGte() override {}

private:
    const ObjectId mLeft;
    const ObjectId mRight;
    const ObjectId mTarget;
};


class JumpIf: public Instruction
{
public:
    JumpIf(ObjectId condition, InstructionPointer ipNew);
    const char * name() const override { return "JumpIf"; }
    void call(std::vector<Object> & data, InstructionPointer & ip) const override;
    ~JumpIf() override {}

private:
    const ObjectId mCondition;
    const InstructionPointer mIpNew;
};


class Jump: public Instruction
{
public:
    Jump(InstructionPointer ipNew);
    const char * name() const override { return "Jump"; }
    void call(std::vector<Object> & data, InstructionPointer & ip) const override;
    ~Jump() override {}

private:
    const InstructionPointer mIpNew;
};


class Copy: public Instruction
{
public:
    Copy(ObjectId source, ObjectId target);
    const char * name() const override { return "Copy"; }
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
    const char * name() const override { return "IntLessThan"; }
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
    const char * name() const override { return "IntLTE"; }
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
    const char * name() const override { return "IsEqual"; }
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
    const char * name() const override { return "IsNotEqual"; }
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
    const char * name() const override { return "IntGTE"; }
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
    const char * name() const override { return "IntGreaterThan"; }
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
    const char * name() const override { return "OrTest"; }
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
    const char * name() const override { return "AndTest"; }
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
    const char * name() const override { return "Negate"; }
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
    const char * name() const override { return "CallFunction"; }
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
    const char * name() const override { return "Noop"; }
    void call(std::vector<Object> & data, InstructionPointer & ip) const override;
    ~Noop() override {}
};



} // namespace instructions
