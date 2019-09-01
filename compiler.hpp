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

private:

    ObjectId freshObjectId();
    bool lookup(const std::string & name);

    ObjectId nextObjectId { 0 };
    std::vector<Instruction> mInstructions;
    ObjectId latestObjectId = -1;
    std::unordered_map<ObjectId, ObjectType> mTypes;
    std::unordered_map<std::string, ObjectId> mLookup;
};
