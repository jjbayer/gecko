#include "ast.hpp"
#include "compiler.hpp"
#include "exceptions.hpp"


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

    const auto created = lookupOrCreate(name->mName);
    const auto sourceType = mTypes[sourceId];
    if( ! created && mTypes[latestObjectId] != sourceType ) {
        throw TypeMismatch(assignment.position(), ""); // TODO: mPosition, text
    }
    mTypes[latestObjectId] = sourceType;

    mInstructions.push_back(copy(sourceId, latestObjectId));
}

void Compiler::visitFunctionCall(const ast::FunctionCall &functionCall)
{
    std::vector<ObjectId> ids;
    for( const auto & arg : functionCall.mArguments ) {
        arg->acceptVisitor(*this);
        ids.push_back(latestObjectId);
    }

    if( functionCall.mName->mName == "print" && ids.size() == 1 && mTypes[ids[0]] == ObjectType::INT ) {
        mInstructions.push_back(printInt(ids[0]));
    } else {
        throw std::runtime_error("Only single argument int printing supported right now");
    }
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
    lookup(name.mName); // sets latest object id
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

void Compiler::lookup(const std::string &name)
{
    latestObjectId = mLookup.lookup(name);
}

bool Compiler::lookupOrCreate(const std::string &name)
{
    bool created;
    std::tie(latestObjectId, created) = mLookup.lookupOrCreate(name);

    return created;
}

InstructionPointer Compiler::latestInstructionPointer() const
{
    return mInstructions.size() - 1;
}
