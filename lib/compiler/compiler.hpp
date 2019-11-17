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
    void visitFree() override;
    void visitFunctionCall(const ast::FunctionCall &functionCall) override;
    void visitIfThen(const ast::IfThen &ifThenElse) override;
    void visitIfThenElse(const ast::IfThenElse &ifThenElse) override;
    void visitIntLiteral(const ast::IntLiteral & literal) override;
    void visitName(const ast::Name &name) override;
    void visitOr(const ast::Or & test) override;
    void visitScope(const ast::Scope &scope) override;
    void visitStringLiteral(const ast::StringLiteral & visitable) override;
    void visitWhile(const ast::While &loop) override;

private:

    void loadPrelude();

    template<typename T>
    void registerBuiltinFunction(const std::string & name)
    {
        T dummy;

        // TODO: no need to lookup
        lookupOrCreate({name, dummy.argumentTypes()});
        latestObject.type = mTypeCreator.functionType(dummy.returnType(), dummy.argumentTypes());
        latestObject.returnType = dummy.returnType();
        mInstructions.push_back(
            std::make_unique<instructions::SetFunction>(latestObject.id, &std::make_unique<T>)
        );
    }

    // type
    // lookup key: name, argument_types
    // return type: again a type

    void lookup(const ast::Name & name, const std::vector<Type> & argumentTypes);
    bool lookupOrCreate(const LookupKey & key);
    InstructionPointer latestInstructionPointer() const;

    std::vector<std::unique_ptr<Instruction> > mInstructions;
    ObjectProvider mObjectProvider;
    CompileTimeObject latestObject;
    Lookup mLookup;
    TypeCreator mTypeCreator;
};
