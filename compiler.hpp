#pragma once

#include "instructions.hpp"
#include "visitor.hpp"

#include <unordered_map>


enum class ObjectType
{
    INT = 0
};


class Compiler: public ast::Visitor
{

public:

    const std::vector<Instruction> & instructions() const;

    void visitAddition(const ast::Addition &addition) override;
    void visitAssignment(const ast::Assignment &assignment) override;
    void visitFunctionCall(const ast::FunctionCall &functionCall) override;
    void visitIntLiteral(const ast::IntLiteral &intLiteral) override;
    void visitName(const ast::Name &name) override;
    void visitScope(const ast::Scope &scope) override;
    void visitWhile(const ast::While &loop) override;
    void visitLessThan(const ast::LessThan &intLessThan) override;

private:

    ObjectId freshObjectId();
    void lookup(const std::string & name);
    bool lookupOrCreate(const std::string & name);
    InstructionPointer latestInstructionPointer() const;

    ObjectId nextObjectId { 0 };
    std::vector<Instruction> mInstructions;
    ObjectId latestObjectId = -1;
    std::unordered_map<ObjectId, ObjectType> mTypes;
    std::unordered_map<std::string, ObjectId> mLookup;

};
