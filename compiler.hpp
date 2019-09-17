#pragma once

#include "lookup.hpp"
#include "instructions.hpp"
#include "visitor.hpp"

#include <unordered_map>


class Compiler: public ast::Visitor
{

public:

    const std::vector<Instruction> & instructions() const;
    int numObjectIdsUsed() const { return mLookup.numObjectIdsUsed(); }

    void visitAddition(const ast::Addition &addition) override;
    void visitAssignment(const ast::Assignment &assignment) override;
    void visitFunctionCall(const ast::FunctionCall &functionCall) override;
    void visitIntLiteral(const ast::IntLiteral & literal) override;
    void visitFloatLiteral(const ast::FloatLiteral & literal) override;
    void visitName(const ast::Name &name) override;
    void visitScope(const ast::Scope &scope) override;
    void visitWhile(const ast::While &loop) override;
    void visitLessThan(const ast::LessThan &intLessThan) override;
    void visitIfThen(const ast::IfThen &ifThenElse) override;
    void visitIfThenElse(const ast::IfThenElse &ifThenElse) override;

private:


    void lookup(const LookupKey & key);
    bool lookupOrCreate(const LookupKey & key);
    InstructionPointer latestInstructionPointer() const;

    std::vector<Instruction> mInstructions;
    ObjectId latestObjectId = -1;
    std::unordered_map<ObjectId, ObjectType> mTypes;
    Lookup mLookup;

};
