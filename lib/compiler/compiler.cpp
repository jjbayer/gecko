#include "compiler.hpp"
#include "parser/ast.hpp"
#include "common/exceptions.hpp"
#include "common/builtins.hpp"

#include <sstream>


Compiler::Compiler()
{
    loadPrelude();
}

const std::vector<std::unique_ptr<Instruction> > &Compiler::instructions() const
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
    if( mTypes.at(lhs) != ObjectType::INT || mTypes.at(rhs) != ObjectType::INT) {
        throw TypeMismatch(addition.position(), ""); // TODO: mPosition, text
    }

    latestObjectId = mLookup.freshObjectId();
    mTypes[latestObjectId] = mTypes.at(lhs);

    mInstructions.push_back(std::make_unique<instructions::AddInt>(lhs, rhs, latestObjectId));
}

void Compiler::visitAssignment(const ast::Assignment &assignment)
{
    assignment.mValue->acceptVisitor(*this);
    const auto sourceId = latestObjectId;

    // TODO: what if assignee is not name?
    auto name = dynamic_cast<ast::Name*>(assignment.mAssignee.get());

    const auto created = lookupOrCreate({name->mName, {}});
    const auto targetId = latestObjectId;
    const auto sourceType = mTypes.at(sourceId);
    if( ! created && mTypes.at(targetId) != sourceType ) {
        throw TypeMismatch(assignment.position(), ""); // TODO: mPosition, text
    }
    mTypes[targetId] = sourceType;

    mInstructions.push_back(std::make_unique<instructions::Copy>(sourceId, targetId));
}

void Compiler::visitFunctionCall(const ast::FunctionCall &functionCall)
{
    std::vector<ObjectType> argumentTypes;

    std::vector<ObjectId> originalArgumentIds;
    for( const auto & arg : functionCall.mArguments ) {
        arg->acceptVisitor(*this);
        originalArgumentIds.push_back(latestObjectId);
        argumentTypes.push_back(mTypes.at(latestObjectId));
    }

    lookup(*functionCall.mName, argumentTypes);
    const auto functionId = latestObjectId;

    ObjectId firstArg = 0;
    for( const auto originalArgumentId : originalArgumentIds ) {
        const auto argumentId = mLookup.freshObjectId();
        if(  firstArg == 0 ) firstArg = argumentId;
        // TODO: memory management
        mInstructions.push_back(std::make_unique<instructions::Copy>(originalArgumentId, argumentId));
    }

    const auto returnValueId = latestObjectId = mLookup.freshObjectId();

    mTypes[returnValueId] = mTypes.at(functionId); // type of function is type of return value

    mInstructions.push_back(std::make_unique<instructions::CallFunction>(functionId, firstArg, returnValueId));
}

void Compiler::visitIntLiteral(const ast::IntLiteral &literal)
{
    latestObjectId = mLookup.freshObjectId();
    mTypes[latestObjectId] = ObjectType::INT;
    mInstructions.push_back(std::make_unique<instructions::SetInt>(latestObjectId, literal.mValue));
}

void Compiler::visitFloatLiteral(const ast::FloatLiteral &literal)
{
    latestObjectId = mLookup.freshObjectId();
    mTypes[latestObjectId] = ObjectType::FLOAT;
    mInstructions.push_back(std::make_unique<instructions::SetFloat>(latestObjectId, literal.mValue));
}

void Compiler::visitBooleanLiteral(const ast::BooleanLiteral &literal)
{
    latestObjectId = mLookup.freshObjectId();
    mTypes[latestObjectId] = ObjectType::BOOLEAN;
    mInstructions.push_back(std::make_unique<instructions::SetBoolean>(latestObjectId, literal.mValue));
}

void Compiler::visitComparison(const ast::Comparison &visitable)
{
    auto lastTestResult = mLookup.freshObjectId();
    mTypes[lastTestResult] = ObjectType::BOOLEAN;
    mInstructions.push_back(std::make_unique<instructions::SetBoolean>(lastTestResult, true));

    // TODO: short circuiting
    for( size_t i = 0; i < visitable.mOperators.size(); i++ ) {
        visitable.mOperands.at(i)->acceptVisitor(*this);
        const auto lhs = latestObjectId;
        visitable.mOperands.at(i+1)->acceptVisitor(*this);
        const auto rhs = latestObjectId;

        if( mTypes.at(lhs) != mTypes.at(rhs)) {
            throw TypeMismatch(visitable.position(), "Comparison operators must have same type");
        }

        // TODO: allow other types than int
        if( mTypes.at(lhs) != ObjectType::INT ) {
            throw TypeMismatch(visitable.position(), "Only integer comparisons are supported right now");
        }

        const auto testResult = mLookup.freshObjectId();
        mTypes[testResult] = ObjectType::BOOLEAN;

        const auto & op = visitable.mOperators.at(i);
        switch (op) {
        case Token::LessThan:
            mInstructions.push_back(std::make_unique<instructions::IntLessThan>(lhs, rhs, testResult));
            break;
        case Token::LTE:
            mInstructions.push_back(std::make_unique<instructions::IntLTE>(lhs, rhs, testResult));
            break;
        case Token::Equal:
            mInstructions.push_back(std::make_unique<instructions::IsEqual>(lhs, rhs, testResult));
            break;
        case Token::NotEqual:
            mInstructions.push_back(std::make_unique<instructions::IsNotEqual>(lhs, rhs, testResult));
            break;
        case Token::GTE:
            mInstructions.push_back(std::make_unique<instructions::IntGTE>(lhs, rhs, testResult));
            break;
        case Token::GreaterThan:
            mInstructions.push_back(std::make_unique<instructions::IntGreaterThan>(lhs, rhs, testResult));
            break;
        default:
            throw std::runtime_error("Unexpected comparison operator");
            break;
        }

        mInstructions.push_back(std::make_unique<instructions::AndTest>(testResult, lastTestResult, lastTestResult));
    }

    latestObjectId = lastTestResult;
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
    mInstructions.push_back(std::make_unique<instructions::Negate>(condition, jumpCondition));

    mInstructions.push_back(std::make_unique<instructions::Noop>()); // placeholder for jump_if
    const auto ipJumpIf = latestInstructionPointer();

    loop.mBody->acceptVisitor(*this);
    mInstructions.push_back(std::make_unique<instructions::Jump>(ipCondition));

    mInstructions.push_back(std::make_unique<instructions::Noop>()); // Make sure there is something to jump to
    const auto afterLoop = latestInstructionPointer();
    mInstructions[ipJumpIf] = std::make_unique<instructions::JumpIf>(jumpCondition, afterLoop);
}



void Compiler::visitIfThen(const ast::IfThen &ifThen)
{
    ifThen.mCondition->acceptVisitor(*this);
    const auto condition = latestObjectId;
    if( mTypes.at(condition) != ObjectType::BOOLEAN ) {
        throw TypeMismatch(ifThen.mCondition->position(), "If-condition must be boolean");
    }

    const auto negatedCondition = mLookup.freshObjectId();
    mInstructions.push_back(std::make_unique<instructions::Negate>(condition, negatedCondition));

    mInstructions.push_back(std::make_unique<instructions::Noop>());
    const auto ipJumpToEnd = latestInstructionPointer();  // Will hold instruction to jump to end

    ifThen.mIfBlock->acceptVisitor(*this);

    mInstructions.push_back(std::make_unique<instructions::Noop>()); // This is the end
    const auto ipEnd = latestInstructionPointer();

    mInstructions[ipJumpToEnd] = std::make_unique<instructions::JumpIf>(negatedCondition, ipEnd);
}

void Compiler::visitIfThenElse(const ast::IfThenElse &ifThenElse)
{
    ifThenElse.mCondition->acceptVisitor(*this);
    const auto condition = latestObjectId;
    if( mTypes[condition] != ObjectType::BOOLEAN ) {
        throw TypeMismatch(ifThenElse.mCondition->position(), "If-Else condition must be boolean");
    }

    mInstructions.push_back(std::make_unique<instructions::Noop>());
    const auto ipJumpToIf = latestInstructionPointer();  // Will hold instruction to jump to if block

    ifThenElse.mElseBlock->acceptVisitor(*this);
    mInstructions.push_back(std::make_unique<instructions::Noop>());
    const auto ipJumpToEnd = latestInstructionPointer();  // Will hold instruction to jump to end

    mInstructions.push_back(std::make_unique<instructions::Noop>());
    const auto ipStartIfBlock = latestInstructionPointer();

    ifThenElse.mIfBlock->acceptVisitor(*this);

    // TODO: wrap push_back in method which returns instruction pointer
    mInstructions.push_back(std::make_unique<instructions::Noop>()); // This is the end
    const auto ipEnd = latestInstructionPointer();

    mInstructions[ipJumpToIf] = std::make_unique<instructions::JumpIf>(condition, ipStartIfBlock);
    mInstructions[ipJumpToEnd] = std::make_unique<instructions::Jump>(ipEnd);
}

void Compiler::visitOr(const ast::Or &test)
{
    test.mLeft->acceptVisitor(*this);
    const auto lhs = latestObjectId;
    test.mRight->acceptVisitor(*this);
    const auto rhs = latestObjectId;

    // TODO: other forms off addition
    if( mTypes.at(lhs) != ObjectType::BOOLEAN || mTypes.at(rhs) != ObjectType::BOOLEAN) {
        throw TypeMismatch(test.position(), "Both operands of 'or' must be boolean");
    }

    latestObjectId = mLookup.freshObjectId();
    mTypes[latestObjectId] = mTypes.at(lhs);

    mInstructions.push_back(std::make_unique<instructions::OrTest>(lhs, rhs, latestObjectId));
}

void Compiler::visitAnd(const ast::And &test) // TODO: unify operators
{
    test.mLeft->acceptVisitor(*this);
    const auto lhs = latestObjectId;
    test.mRight->acceptVisitor(*this);
    const auto rhs = latestObjectId;

    // TODO: other forms off addition
    if( mTypes.at(lhs) != ObjectType::BOOLEAN || mTypes.at(rhs) != ObjectType::BOOLEAN) {
        throw TypeMismatch(test.position(), "Both operands of 'and' must be boolean");
    }

    latestObjectId = mLookup.freshObjectId();
    mTypes[latestObjectId] = mTypes.at(lhs);

    mInstructions.push_back(std::make_unique<instructions::AndTest>(lhs, rhs, latestObjectId));
}

void Compiler::loadPrelude()
{
    registerBuiltinFunction(new PrintInt, "print");
    registerBuiltinFunction(new AddInt, "__add__"); // FIXME: memory management
    registerBuiltinFunction(new Dummy, "dummy");
}

void Compiler::registerBuiltinFunction(obj::Function * func, const std::string &name)
{
    bool created;
    std::tie(latestObjectId, created) = mLookup.lookupOrCreate({name, func->argumentTypes()});
    mTypes[latestObjectId] = func->returnType();
    // FIXME: memory management
    mInstructions.push_back(std::make_unique<instructions::SetFunction>(latestObjectId, func));
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
