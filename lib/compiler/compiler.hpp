#pragma once

#include "lookup.hpp"
#include "objectprovider.hpp"
#include "runtime/instructions.hpp"
#include "parser/visitor.hpp"

#include <memory>
#include <unordered_map>


class Compiler: public ast::Visitor
{

public:

    Compiler();

    const std::vector<std::unique_ptr<Instruction> > & instructions() const;
    int numObjectIdsUsed() const { return mObjectProvider.numObjectsIssued(); }

    void visitAddition(const ast::Addition &addition) override;
    void visitAnd(const ast::And & test) override;
    void visitAssignment(const ast::Assignment &assignment) override;
    void visitBooleanLiteral(const ast::BooleanLiteral & literal) override;
    void visitComparison(const ast::Comparison &visitable) override;
    void visitFloatLiteral(const ast::FloatLiteral & literal) override;
    void visitFunctionCall(const ast::FunctionCall &functionCall) override;
    void visitIfThen(const ast::IfThen &ifThenElse) override;
    void visitIfThenElse(const ast::IfThenElse &ifThenElse) override;
    void visitIntLiteral(const ast::IntLiteral & literal) override;
    void visitName(const ast::Name &name) override;
    void visitOr(const ast::Or & test) override;
    void visitScope(const ast::Scope &scope) override;
    void visitWhile(const ast::While &loop) override;

private:

    void loadPrelude();
    void registerBuiltinFunction(obj::Function *func, const std::string & name);

    // type
    // lookup key: name, argument_types
    // return type: again a type

    void lookup(const ast::Name & name, const std::vector<ValueType> & argumentTypes);
    bool lookupOrCreate(const LookupKey & key);
    InstructionPointer latestInstructionPointer() const;

    std::vector<std::unique_ptr<Instruction> > mInstructions;
    CompileTimeObject latestObject;
    ObjectProvider mObjectProvider;
    Lookup mLookup;

};
