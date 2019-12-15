#include "compiler.hpp"
#include "parser/ast.hpp"
#include "common/exceptions.hpp"
#include "runtime/objects/builtins.hpp"
#include "runtime/objects/stdin.hpp"

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
    const auto lhs = latestObject;
    addition.mRight->acceptVisitor(*this);
    const auto rhs = latestObject;

    // TODO: other forms off addition
    if( lhs->type != BasicType::INT || rhs->type != BasicType::INT) {
        throw TypeMismatch(addition.position(), ""); // TODO: mPosition, text
    }

    latestObject = mObjectProvider.createObject(lhs->type);

    appendInstruction<instructions::AddInt>(lhs->id, rhs->id, latestObject->id);
}

void Compiler::visitAssignment(const ast::Assignment &assignment)
{
    assignment.mValue->acceptVisitor(*this);
    const auto source = latestObject;

    // TODO: what if assignee is not name?
    auto name = dynamic_cast<ast::Name*>(assignment.mAssignee.get());

    const LookupKey lookupKey {name->mName};
    const auto created = lookupOrCreate(lookupKey);
    auto destination = latestObject;
    if( ! created && destination->type != source->type ) {
        throw TypeMismatch(assignment.position(), ""); // TODO: mPosition, text
    }
    destination->type = source->type;

    appendInstruction<instructions::Copy>(source->id, destination->id);
}

void Compiler::visitFunctionCall(const ast::FunctionCall &functionCall)
{
    std::vector<Type> argumentTypes;

    std::vector<ObjectId> originalArgumentIds;
    for( const auto & arg : functionCall.mArguments ) {
        arg->acceptVisitor(*this);
        originalArgumentIds.push_back(latestObject->id);
        argumentTypes.push_back(latestObject->type);
    }

    lookup(*functionCall.mName, argumentTypes);
    const auto function = latestObject;

    ObjectId firstArg = 0;
    for( const auto originalArgumentId : originalArgumentIds ) {
        const auto argument = mObjectProvider.createObject();
        if(  firstArg == 0 ) firstArg = argument->id;
        appendInstruction<instructions::Copy>(originalArgumentId, argument->id);
    }

    latestObject = mObjectProvider.createObject(function->returnType); // type of function is type of return value

    appendInstruction<instructions::CallFunction>(function->id, firstArg, latestObject->id);
}

void Compiler::visitIntLiteral(const ast::IntLiteral &literal)
{
    latestObject = mObjectProvider.createObject(BasicType::INT);
    appendInstruction<instructions::SetInt>(latestObject->id, literal.mValue);
}

void Compiler::visitFloatLiteral(const ast::FloatLiteral &literal)
{
    latestObject = mObjectProvider.createObject(BasicType::FLOAT);
    appendInstruction<instructions::SetFloat>(latestObject->id, literal.mValue);
}

void Compiler::visitFor(const ast::For &loop)
{
    loop.mRange->acceptVisitor(*this);
    const auto range = latestObject;
    auto nextFn = mLookup.lookup({"next", {range->type}});

    if( ! nextFn ) throw UndefinedVariable(loop.mRange->position(), "next");

    auto optional = mObjectProvider.createObject(nextFn->returnType);
    auto itemType = getOptionalType(mTypeCreator, optional->type);

    // Create new address & special scope for loop var:
    auto loopVar = mObjectProvider.createObject(itemType);
    mLookup.push();
    mLookup.set(loop.mLoopVariable->mName, loopVar);

    auto expectedEnumKey = mObjectProvider.createObject(BasicType::INT);
    appendInstruction<instructions::SetInt>(expectedEnumKey->id, 1);

    appendInstruction<instructions::CallFunction>(nextFn->id, range->id, optional->id);
    const auto ipNext = latestInstructionPointer();

    // TODO: Visit enum
    auto enumKey = mObjectProvider.createObject(BasicType::INT);
    appendInstruction<instructions::ReadFromTuple<0, 2> >(optional->id, enumKey->id);
    auto condition = mObjectProvider.createObject(BasicType::BOOLEAN);
    appendInstruction<instructions::IsEqual>(enumKey->id, expectedEnumKey->id, condition->id);

    appendInstruction<instructions::Noop>(); // placeholder for jump_if
    const auto ipJumpIfNot = latestInstructionPointer();

    // Now we are in the section where optional has value
    appendInstruction<instructions::ReadFromTuple<1, 2> >(optional->id, loopVar->id);

    loop.mBody->acceptVisitor(*this);
    appendInstruction<instructions::Jump>(ipNext);

    appendInstruction<instructions::Noop>(); // Make sure there is something to jump to
    const auto afterLoop = latestInstructionPointer();
    mInstructions[ipJumpIfNot] = std::make_unique<instructions::JumpIfNot>(condition->id, afterLoop);

    mLookup.pop();
}

void Compiler::visitFree()
{
    std::vector<ObjectId> objectsInUse;
    for(const auto & scope : mLookup.scopes()) {
        for( const auto & pair : scope ) {
            const auto & object = pair.second;
            if( object->isAllocated() ) {
                objectsInUse.push_back(object->id);
            }
        }
    }

    appendInstruction<instructions::CollectGarbage>(objectsInUse);
}

void Compiler::visitBooleanLiteral(const ast::BooleanLiteral &literal)
{
    latestObject = mObjectProvider.createObject(BasicType::BOOLEAN);
    appendInstruction<instructions::SetBoolean>(latestObject->id, literal.mValue);
}

void Compiler::visitComparison(const ast::Comparison &visitable)
{
    if( visitable.mOperands.size() < 1 ) {
        throw CompilerBug("Expected at least one comparison");
    }

    std::shared_ptr<CompileTimeObject> lastTestResult = nullptr;

    // TODO: short circuiting
    for( size_t i = 0; i < visitable.mOperators.size(); i++ ) {
        visitable.mOperands.at(i)->acceptVisitor(*this);
        const auto lhs = latestObject;
        visitable.mOperands.at(i+1)->acceptVisitor(*this);
        const auto rhs = latestObject;

        if( lhs->type != rhs->type) {
            throw TypeMismatch(visitable.position(), "Comparison operators must have same type");
        }

        // TODO: allow other types than int
        if( lhs->type != BasicType::INT ) {
            throw TypeMismatch(visitable.position(), "Only integer comparisons are supported right now");
        }

        auto testResult = mObjectProvider.createObject();
        testResult->type = BasicType::BOOLEAN;

        const auto & op = visitable.mOperators.at(i);
        switch (op) {
        case Token::LessThan:
            appendInstruction<instructions::IntLessThan>(lhs->id, rhs->id, testResult->id);
            break;
        case Token::LTE:
            appendInstruction<instructions::IntLTE>(lhs->id, rhs->id, testResult->id);
            break;
        case Token::Equal:
            appendInstruction<instructions::IsEqual>(lhs->id, rhs->id, testResult->id);
            break;
        case Token::NotEqual:
            appendInstruction<instructions::IsNotEqual>(lhs->id, rhs->id, testResult->id);
            break;
        case Token::GTE:
            appendInstruction<instructions::IntGTE>(lhs->id, rhs->id, testResult->id);
            break;
        case Token::GreaterThan:
            appendInstruction<instructions::IntGreaterThan>(lhs->id, rhs->id, testResult->id);
            break;
        default:
            throw std::runtime_error("Unexpected comparison operator");
        }

        if( lastTestResult ) {
            appendInstruction<instructions::AndTest>(testResult->id, lastTestResult->id, lastTestResult->id);
        } else {
            lastTestResult = testResult;
        }
    }

    latestObject = lastTestResult;
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

void Compiler::visitStringLiteral(const ast::StringLiteral &visitable)
{
    latestObject = mObjectProvider.createObject(BasicType::STRING);
    appendInstruction<instructions::SetString>(latestObject->id, visitable.mValue);
}


void Compiler::visitWhile(const ast::While &loop)
{
    const auto ipStartOfCondition = latestInstructionPointer() + 1;

    loop.mCondition->acceptVisitor(*this);
    auto condition = latestObject;
    if( condition->type != BasicType::BOOLEAN ) {
        throw TypeMismatch(loop.position(), "While condition must be boolean"); // TODO: mPosition, text
    }

    appendInstruction<instructions::Noop>(); // placeholder for jump_if
    const auto ipJumpIfNot = latestInstructionPointer();

    loop.mBody->acceptVisitor(*this);
    appendInstruction<instructions::Jump>(ipStartOfCondition);

    appendInstruction<instructions::Noop>(); // Make sure there is something to jump to
    const auto afterLoop = latestInstructionPointer();
    mInstructions[ipJumpIfNot] = std::make_unique<instructions::JumpIfNot>(condition->id, afterLoop);
}



void Compiler::visitIfThen(const ast::IfThen &ifThen)
{
    ifThen.mCondition->acceptVisitor(*this);
    auto condition = latestObject;
    if( condition->type != BasicType::BOOLEAN ) {
        throw TypeMismatch(ifThen.mCondition->position(), "If-condition must be boolean");
    }

    appendInstruction<instructions::Noop>();
    const auto ipJumpToEnd = latestInstructionPointer();  // Will hold instruction to jump to end

    ifThen.mIfBlock->acceptVisitor(*this);

    appendInstruction<instructions::Noop>(); // This is the end
    const auto ipEnd = latestInstructionPointer();

    mInstructions[ipJumpToEnd] = std::make_unique<instructions::JumpIfNot>(condition->id, ipEnd);
}

void Compiler::visitIfThenElse(const ast::IfThenElse &ifThenElse)
{
    ifThenElse.mCondition->acceptVisitor(*this);
    const auto condition = latestObject;
    if( condition->type != BasicType::BOOLEAN ) {
        throw TypeMismatch(ifThenElse.mCondition->position(), "If-Else condition must be boolean");
    }

    appendInstruction<instructions::Noop>();
    const auto ipJumpToIf = latestInstructionPointer();  // Will hold instruction to jump to if block

    ifThenElse.mElseBlock->acceptVisitor(*this);
    appendInstruction<instructions::Noop>();
    const auto ipJumpToEnd = latestInstructionPointer();  // Will hold instruction to jump to end

    appendInstruction<instructions::Noop>();
    const auto ipStartIfBlock = latestInstructionPointer();

    ifThenElse.mIfBlock->acceptVisitor(*this);

    // TODO: wrap push_back in method which returns instruction pointer
    appendInstruction<instructions::Noop>(); // This is the end
    const auto ipEnd = latestInstructionPointer();

    mInstructions[ipJumpToIf] = std::make_unique<instructions::JumpIf>(condition->id, ipStartIfBlock);
    mInstructions[ipJumpToEnd] = std::make_unique<instructions::Jump>(ipEnd);
}

void Compiler::visitOr(const ast::Or &test)
{
    test.mLeft->acceptVisitor(*this);
    const auto lhs = latestObject;
    test.mRight->acceptVisitor(*this);
    const auto rhs = latestObject;

    // TODO: other forms off addition
    if( lhs->type != BasicType::BOOLEAN || rhs->type != BasicType::BOOLEAN) {
        throw TypeMismatch(test.position(), "Both operands of 'or' must be boolean");
    }

    latestObject = mObjectProvider.createObject(lhs->type);

    appendInstruction<instructions::OrTest>(lhs->id, rhs->id, latestObject->id);
}

void Compiler::visitAnd(const ast::And &test) // TODO: unify operators
{
    test.mLeft->acceptVisitor(*this);
    const auto lhs = latestObject;
    test.mRight->acceptVisitor(*this);
    const auto rhs = latestObject;

    // TODO: other forms off addition
    if( lhs->type != BasicType::BOOLEAN || rhs->type != BasicType::BOOLEAN) {
        throw TypeMismatch(test.position(), "Both operands of 'and' must be boolean");
    }

    latestObject = mObjectProvider.createObject(lhs->type);

    appendInstruction<instructions::AndTest>(lhs->id, rhs->id, latestObject->id);
}

void Compiler::loadPrelude()
{
    registerBuiltinFunction<PrintInt>("print");
    registerBuiltinFunction<PrintString>("print");
    registerBuiltinFunction<AddInt>("__add__");
    registerBuiltinFunction<Dummy>("dummy");

    lookupOrCreate({"stdin"}); // TODO: no need to lookup
    latestObject->type = mTypeCreator.structType("Stdin");
    registerBuiltinFunction<obj::NextStdin>("next");
}

void Compiler::lookup(const ast::Name &name)
{
    if( auto object =  mLookup.lookup({name.mName}) ) {
        latestObject = object;
    } else {
        throw UndefinedVariable(name.position(), name.mName);
    }
}




void Compiler::lookup(const ast::Name &variable, const std::vector<Type> &argumentTypes)
{
    if( auto object = mLookup.lookup({variable.mName, argumentTypes})) {
        latestObject = object;
    } else {
        // TODO: readable argument types
        // TODO: key.is_function
        std::stringstream msg;
        msg << variable.mName;
        if( ! argumentTypes.empty() ) msg << " with function arguments of types ";
        // TODO: add type to string
        for(auto arg : argumentTypes) msg << arg << " ";

        throw UndefinedVariable(variable.position(), msg.str());
    }
}

bool Compiler::lookupOrCreate(const LookupKey &key)
{
    if( auto object = mLookup.lookup(key) ) {

        latestObject = object;

        return false;

    } else {

        latestObject = mObjectProvider.createObject();
        mLookup.set(key, latestObject);

        return true;
    }
}


InstructionPointer Compiler::latestInstructionPointer() const
{
    return mInstructions.size() - 1;
}
