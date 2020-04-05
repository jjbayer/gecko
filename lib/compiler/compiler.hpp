#pragma once

#include "lookup.hpp"
#include "objectprovider.hpp"
#include "runtime/instructions.hpp"
#include "parser/visitor.hpp"

#include <memory>
#include <unordered_map>


namespace ct {


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
    void visitFor(const ast::For & loop) override;
    void visitFree() override;
    void visitFunctionCall(const ast::FunctionCall &functionCall) override;
    void visitFunctionDefinition(const ast::FunctionDefinition &functionCall) override;
    void visitIfThen(const ast::IfThen &ifThenElse) override;
    void visitIfThenElse(const ast::IfThenElse &ifThenElse) override;
    void visitIntLiteral(const ast::IntLiteral & literal) override;
    void visitName(const ast::Name &name) override;
    void visitOr(const ast::Or & test) override;
    void visitScope(const ast::Scope &scope) override;
    void visitStringLiteral(const ast::StringLiteral & visitable) override;
    void visitTypeName(const ast::TypeName & visitable) override;
    void visitWhile(const ast::While &loop) override;

private:

    void loadPrelude();

    template<typename T>
    void registerBuiltinFunction(const std::string & name)
    {
        T dummy;

        mLookup.setFunction({name, dummy.argumentTypes()}, std::make_unique<T>());
    }

    void lookupObject(const ast::Name & name);

    void lookupType(const ast::TypeName & typeName);

    bool lookupOrCreate(const std::string & key);
    InstructionPointer latestInstructionPointer() const;

    template<typename T, typename ... Args>
    void appendInstruction(Args && ... args)
    {
        mInstructions.push_back(std::make_unique<T>(std::forward<Args>(args)...));
    }

    std::vector<std::unique_ptr<Instruction> > mInstructions;
    ObjectProvider mObjectProvider;
    std::shared_ptr<CompileTimeObject> latestObject = nullptr;
    Type latestType = BasicType::NONE;
    Lookup mLookup;
    TypeCreator & mTypeCreator = typeCreator(); // TODO: no globals
};

} // namespace ct
