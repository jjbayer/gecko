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
        throw TypeMismatch();
    }

    latestObjectId = freshObjectId();
    mTypes[latestObjectId] = mTypes[lhs];

    mInstructions.push_back(addInt(lhs, rhs, latestObjectId));
}

void Compiler::visitAssignment(const ast::Assignment &assignment)
{
    assignment.mValue->acceptVisitor(*this);
    const auto sourceId = latestObjectId;

    const auto created = lookup(assignment.mName->mName);
    const auto sourceType = mTypes[sourceId];
    if( ! created && mTypes[latestObjectId] != sourceType ) {
        throw TypeMismatch(); // TODO: info
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

void Compiler::visitIntLiteral(const ast::IntLiteral &intLiteral)
{
    latestObjectId = freshObjectId();
    mTypes[latestObjectId] = ObjectType::INT;
    mInstructions.push_back(setInt(latestObjectId, intLiteral.mValue));
}

void Compiler::visitName(const ast::Name &name)
{
    // FIXME: this creates a variable even outside assignment
    lookup(name.mName); // sets latest object id
}

void Compiler::visitScope(const ast::Scope &scope)
{
    // TODO: actually support nested scopes
    for(auto & statement : scope.mStatements) {
        statement->acceptVisitor(*this);
    }
}

void Compiler::visitWhile(const ast::While &loop)
{
    loop.mCondition->acceptVisitor(*this);
    const auto condition = latestObjectId;
    if( mTypes[condition] != ObjectType::INT ) {
        throw TypeMismatch();
    }
    const auto ipCondition = latestInstructionPointer();

    const auto jumpCondition = freshObjectId();
    // TODO: negate condition at compile time, not runtime
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

    // TODO: other forms off addition
    if( mTypes[lhs] != mTypes[rhs] ) {
        throw TypeMismatch();
    }

    latestObjectId = freshObjectId();
    mTypes[latestObjectId] = mTypes[lhs];

    mInstructions.push_back(intLessThan(lhs, rhs, latestObjectId));
}

ObjectId Compiler::freshObjectId()
{
    latestObjectId = ++nextObjectId;

    return latestObjectId;
}

bool Compiler::lookup(const std::string &name)
{
    const auto it = mLookup.find(name);
    if( it == mLookup.end() ) {
        latestObjectId = mLookup[name] = freshObjectId();

        return true;
    }

    latestObjectId = it->second;

    return false;
}

InstructionPointer Compiler::latestInstructionPointer() const
{
    return mInstructions.size() - 1;
}
