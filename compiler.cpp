#include "ast.hpp"
#include "compiler.hpp"
#include "exceptions.hpp"
#include "builtins.hpp"

#include <sstream>


Compiler::Compiler()
{
    loadPrelude();
}

const std::vector<Instruction> &Compiler::instructions() const
{
    return mInstructions;
}

void Compiler::visitAddition(const ast::Addition & addition)
{
    addition.mLeft->acceptVisitor(*this);
    const auto lhs = latestObjectId;
    addition.mRight->acceptVisitor(*this);
    const auto rhs = latestObjectId;

    // TODO: other forms off addition
    if( mTypes[lhs] != ObjectType::INT || mTypes[rhs] != ObjectType::INT) {
        throw TypeMismatch(addition.position(), ""); // TODO: mPosition, text
    }

    latestObjectId = mLookup.freshObjectId();
    mTypes[latestObjectId] = mTypes[lhs];

    mInstructions.push_back(addInt(lhs, rhs, latestObjectId));
}

void Compiler::visitAssignment(const ast::Assignment &assignment)
{
    assignment.mValue->acceptVisitor(*this);
    const auto sourceId = latestObjectId;

    // TODO: what if assignee is not name?
    auto name = dynamic_cast<ast::Name*>(assignment.mAssignee.get());

    const auto created = lookupOrCreate({name->mName, {}});
    const auto targetId = latestObjectId;
    const auto sourceType = mTypes[sourceId];
    if( ! created && mTypes[targetId] != sourceType ) {
        throw TypeMismatch(assignment.position(), ""); // TODO: mPosition, text
    }
    mTypes[targetId] = sourceType;

    mInstructions.push_back(copy(sourceId, targetId));
}

void Compiler::visitFunctionCall(const ast::FunctionCall &functionCall)
{
    std::vector<ObjectId> ids;
    std::vector<ObjectType> argumentTypes;
    for( const auto & arg : functionCall.mArguments ) {
        arg->acceptVisitor(*this);
        ids.push_back(latestObjectId);
        argumentTypes.push_back(mTypes[latestObjectId]);
    }

    lookup(*functionCall.mName, argumentTypes);
    const auto functionId = latestObjectId;
    // FIXME: use mTypes.at(...) everywhere

    const auto returnValueId = mLookup.freshObjectId();

    // FIXME: mTypes[returnValueId] = mReturnTypes.at(functionId);
    mTypes[returnValueId] = ObjectType::INVALID;

    mInstructions.push_back(callFunction(functionId, ids, returnValueId));
}

void Compiler::visitIntLiteral(const ast::IntLiteral &literal)
{
    latestObjectId = mLookup.freshObjectId();
    mTypes[latestObjectId] = ObjectType::INT;
    mInstructions.push_back(setInt(latestObjectId, literal.mValue));
}

void Compiler::visitFloatLiteral(const ast::FloatLiteral &literal)
{
    latestObjectId = mLookup.freshObjectId();
    mTypes[latestObjectId] = ObjectType::FLOAT;
    mInstructions.push_back(setFloat(latestObjectId, literal.mValue));
}

void Compiler::visitName(const ast::Name &name)
{
    lookup(name); // sets latest object id
}

void Compiler::visitScope(const ast::Scope &scope)
{
    mLookup.push();
    for(auto & statement : scope.mStatements) {
        statement->acceptVisitor(*this);
    }
    mLookup.pop();
}

void Compiler::visitWhile(const ast::While &loop)
{
    loop.mCondition->acceptVisitor(*this);
    const auto condition = latestObjectId;
    if( mTypes[condition] != ObjectType::BOOLEAN ) {
        throw TypeMismatch(loop.position(), "While condition must be boolean"); // TODO: mPosition, text
    }
    const auto ipCondition = latestInstructionPointer();

    const auto jumpCondition = mLookup.freshObjectId();
    mInstructions.push_back(negateInt(condition, jumpCondition));

    mInstructions.push_back(noop()); // placeholder for jump_if
    const auto ipJumpIf = latestInstructionPointer();

    loop.mBody->acceptVisitor(*this);
    mInstructions.push_back(jump(ipCondition));

    mInstructions.push_back(noop()); // Make sure there is something to jump to
    const auto afterLoop = latestInstructionPointer();
    mInstructions[ipJumpIf] = jumpIf(jumpCondition, afterLoop);
}

void Compiler::visitLessThan(const ast::LessThan &lessThan)
{
    lessThan.mLeft->acceptVisitor(*this);
    const auto lhs = latestObjectId;
    lessThan.mRight->acceptVisitor(*this);
    const auto rhs = latestObjectId;

    if( mTypes[lhs] != ObjectType::INT ) {
        throw TypeMismatch(lessThan.mLeft->position(), "Only integers can be compared at the moment");
    }

    if( mTypes[lhs] != mTypes[rhs] ) {
        throw TypeMismatch(lessThan.mRight->position(), "Can only compare objects of same type"); // TODO: mPosition, text
    }

    latestObjectId = mLookup.freshObjectId();
    mTypes[latestObjectId] = ObjectType::BOOLEAN;

    mInstructions.push_back(intLessThan(lhs, rhs, latestObjectId));
}

void Compiler::visitIfThen(const ast::IfThen &ifThen)
{
    ifThen.mCondition->acceptVisitor(*this);
    const auto condition = latestObjectId;
    if( mTypes[condition] != ObjectType::BOOLEAN ) {
        throw TypeMismatch(ifThen.mCondition->position(), "If-condition must be boolean"); // TODO: mPosition, text
    }

    const auto negatedCondition = mLookup.freshObjectId();
    mInstructions.push_back(negateInt(condition, negatedCondition));

    mInstructions.push_back(noop());
    const auto ipJumpToEnd = latestInstructionPointer();  // Will hold instruction to jump to end

    ifThen.mIfBlock->acceptVisitor(*this);

    mInstructions.push_back(noop()); // This is the end
    const auto ipEnd = latestInstructionPointer();

    mInstructions[ipJumpToEnd] = jumpIf(negatedCondition, ipEnd);
}

void Compiler::visitIfThenElse(const ast::IfThenElse &ifThenElse)
{
    ifThenElse.mCondition->acceptVisitor(*this);
    const auto condition = latestObjectId;
    if( mTypes[condition] != ObjectType::BOOLEAN ) {
        throw TypeMismatch(ifThenElse.mCondition->position(), "If-Else condition must be boolean"); // TODO: mPosition, text
    }

    mInstructions.push_back(noop());
    const auto ipJumpToIf = latestInstructionPointer();  // Will hold instruction to jump to if block

    ifThenElse.mElseBlock->acceptVisitor(*this);
    mInstructions.push_back(noop());
    const auto ipJumpToEnd = latestInstructionPointer();  // Will hold instruction to jump to end

    mInstructions.push_back(noop());
    const auto ipStartIfBlock = latestInstructionPointer();

    ifThenElse.mIfBlock->acceptVisitor(*this);

    // TODO: wrap push_back in method which returns instruction pointer
    mInstructions.push_back(noop()); // This is the end
    const auto ipEnd = latestInstructionPointer();

    mInstructions[ipJumpToIf] = jumpIf(condition, ipStartIfBlock);
    mInstructions[ipJumpToEnd] = jump(ipEnd);
}

void Compiler::loadPrelude()
{
    bool created;
    std::tie(latestObjectId, created) = mLookup.lookupOrCreate({"print", {ObjectType::INT}});
    mTypes[latestObjectId] = ObjectType::FUNCTION;
    // FIXME: memory management
    mInstructions.push_back(setFunction(latestObjectId, new PrintInt));
}

void Compiler::lookup(const ast::Name &variable, const std::vector<ObjectType> &argumentTypes)
{
    try {
        latestObjectId = mLookup.lookup({variable.mName, argumentTypes});
    } catch(const LookupError &) {

        // TODO: readable argument types
        // TODO: key.is_function
        std::stringstream msg;
        msg << variable.mName;
        if( ! argumentTypes.empty() ) msg << " with function arguments ";
        for(auto arg : argumentTypes) msg << arg << " ";

        throw UndefinedVariable(variable.position(), msg.str());
    }
}

bool Compiler::lookupOrCreate(const LookupKey &key)
{
    bool created;
    std::tie(latestObjectId, created) = mLookup.lookupOrCreate(key);

    return created;
}

InstructionPointer Compiler::latestInstructionPointer() const
{
    return mInstructions.size() - 1;
}
