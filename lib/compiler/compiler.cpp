#include "compiler.hpp"
#include "parser/ast.hpp"
#include "common/exceptions.hpp"
#include "functionkey.hpp"
#include "functions/builtins.hpp"
#include "functions/stdin.hpp"
#include "functions/userfunction.hpp"
#include "compiletimeobject.hpp"

#include<memory>
#include <sstream>


namespace ct {

Compiler::Compiler()
{
    loadPrelude();
}

const InstructionVector &Compiler::instructions() const
{
    return mInstructions;
}

void Compiler::visitAddition(const ast::Addition & addition)
{
    addition.mLeft->acceptVisitor(*this);
    const auto lhs = latestObject;
    addition.mRight->acceptVisitor(*this);
    const auto rhs = latestObject;

    // TODO: Replace by function call __add__
    if( lhs->type != BasicType::INT || rhs->type != BasicType::INT) {
        throw TypeMismatch(addition.position(), ""); // TODO: mPosition, text
    }

    latestObject = mObjectProvider.createObject(lhs->type);

    appendInstruction<ins::AddInt>(lhs->id, rhs->id, latestObject->id);
}

void Compiler::visitAssignment(const ast::Assignment &assignment)
{
    assignment.mValue->acceptVisitor(*this);
    const auto source = latestObject;

    // TODO: what if assignee is not name?
    auto name = dynamic_cast<ast::Name*>(assignment.mAssignee.get());

    const auto created = lookupOrCreate(name->mName);
    auto destination = latestObject;
    if( ! created && destination->type != source->type ) {
        throw TypeMismatch(assignment.position(), ""); // TODO: mPosition, text
    }
    destination->type = source->type;

    appendInstruction<ins::Copy>(source->id, destination->id);
}

void Compiler::visitFunctionCall(const ast::FunctionCall &functionCall)
{
    std::vector<Type> typeParameters, argumentTypes;

    if( functionCall.mTypeParameters ) {
        for(  const auto & param : functionCall.mTypeParameters->mTypeParameters ) {
            param->acceptVisitor(*this);
            typeParameters.push_back(latestType);
        }
    }

    std::vector<std::shared_ptr<const CompileTimeObject> > arguments;
    for( const auto & arg : functionCall.mArguments ) {
        arg->acceptVisitor(*this);
        arguments.push_back(latestObject);
        argumentTypes.push_back(latestObject->type);
    }

    auto function = lookupFunction(functionCall.mName->mName, typeParameters, argumentTypes, functionCall.position());

    auto returnValue = mObjectProvider.createObject(BasicType::NONE);
    function->generateInstructions(typeParameters, arguments, mInstructions, returnValue);
    latestObject = returnValue;
}


void Compiler::visitFunctionDefinition(const ast::FunctionDefinition & def)
{
    std::vector<Type> argumentTypes;
    std::vector<std::shared_ptr<CompileTimeObject> > argumentSlots;

    // Special scope for arguments
    mLookup.push();

    for(const auto & pair : def.mArguments) {
        lookupType(*pair.second);
        argumentTypes.push_back(latestType);
        lookupOrCreate(pair.first->mName);
        latestObject->type = latestType;
        argumentSlots.push_back(latestObject);
    }

    InstructionVector instructions;
    instructions.push_back(std::make_shared<ins::MemPush>());

    // Compile body, but store instructions in a dedicated vector
    std::swap(instructions, mInstructions);
    def.mBody->acceptVisitor(*this);
    const auto returnObject = latestObject; // Last object touched by function is return value
    std::swap(instructions, mInstructions);

    instructions.push_back(std::make_shared<ins::MemPop>());


    mLookup.pop();

    const auto mTypeParameters = std::vector<Type> {}; // TODO: user functions with type parameters

    const auto functionKey = FunctionKey { def.mName->mName, {}, argumentTypes };

    // Check if exists
    if( mLookup.lookupFunction(functionKey) ) {
        // NOTE: if we ever want template specialization etc., this check might be to strict
        throw FunctionExists(def.position(), def.mName->mName); // TODO: general toString method
    }

    mLookup.setFunction(std::make_unique<ct::UserFunction>(
        functionKey,
        std::move(argumentSlots),
        returnObject,
        std::move(instructions)
    ));
}


void Compiler::visitIntLiteral(const ast::IntLiteral &literal)
{
    latestObject = mObjectProvider.createObject(BasicType::INT);
    appendInstruction<ins::SetInt>(latestObject->id, literal.mValue);
}

void Compiler::visitFloatLiteral(const ast::FloatLiteral &literal)
{
    latestObject = mObjectProvider.createObject(BasicType::FLOAT);
    appendInstruction<ins::SetFloat>(latestObject->id, literal.mValue);
}

void Compiler::visitFor(const ast::For &loop)
{
    loop.mRange->acceptVisitor(*this);
    const auto range = latestObject;

    auto nextFn = lookupFunction("next", {}, {range->type}, loop.mRange->position());

    auto optional = mObjectProvider.createObject();

    // // Create new address & special scope for loop var:
    auto loopVar = mObjectProvider.createObject();
    mLookup.push();
    mLookup.setObject(loop.mLoopVariable->mName, loopVar);

    auto expectedEnumKey = mObjectProvider.createObject(BasicType::INT);
    appendInstruction<ins::SetInt>(expectedEnumKey->id, 1);

    // nextFn
    const auto ipNext = latestInstructionPointer() + 1;
    nextFn->generateInstructions({}, {range}, mInstructions, optional);
    loopVar->type = getOptionalType(mTypeCreator, optional->type);

    // // TODO: Visit enum
    auto enumKey = mObjectProvider.createObject(BasicType::INT);
    appendInstruction<ins::ReadFromTuple<0, 2> >(optional->id, enumKey->id);
    auto condition = mObjectProvider.createObject(BasicType::BOOLEAN);
    appendInstruction<ins::IsEqual>(enumKey->id, expectedEnumKey->id, condition->id);

    appendInstruction<ins::Noop>(); // placeholder for jump_if
    const auto ipJumpIfNot = latestInstructionPointer();

    // Now we are in the section where optional has value
    appendInstruction<ins::ReadFromTuple<1, 2> >(optional->id, loopVar->id);

    loop.mBody->acceptVisitor(*this);
    appendInstruction<ins::Jump>(ipNext);

    appendInstruction<ins::Noop>(); // Make sure there is something to jump to
    const auto afterLoop = latestInstructionPointer();
    mInstructions[ipJumpIfNot] = std::make_unique<ins::JumpIfNot>(condition->id, afterLoop);

    mLookup.pop();
}

void Compiler::visitFree()
{
    std::vector<ObjectId> objectsInUse;
    for(const auto & scope : mLookup.scopes()) {
        for( const auto & pair : scope.mObjects ) {
            const auto & object = pair.second;
            if( object->isAllocated() ) {
                objectsInUse.push_back(object->id);
            }
        }
    }

    appendInstruction<ins::CollectGarbage>(objectsInUse);
}

void Compiler::visitBooleanLiteral(const ast::BooleanLiteral &literal)
{
    latestObject = mObjectProvider.createObject(BasicType::BOOLEAN);
    appendInstruction<ins::SetBoolean>(latestObject->id, literal.mValue);
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
            appendInstruction<ins::IntLessThan>(lhs->id, rhs->id, testResult->id);
            break;
        case Token::LTE:
            appendInstruction<ins::IntLTE>(lhs->id, rhs->id, testResult->id);
            break;
        case Token::Equal:
            appendInstruction<ins::IsEqual>(lhs->id, rhs->id, testResult->id);
            break;
        case Token::NotEqual:
            appendInstruction<ins::IsNotEqual>(lhs->id, rhs->id, testResult->id);
            break;
        case Token::GTE:
            appendInstruction<ins::IntGTE>(lhs->id, rhs->id, testResult->id);
            break;
        case Token::GreaterThan:
            appendInstruction<ins::IntGreaterThan>(lhs->id, rhs->id, testResult->id);
            break;
        default:
            throw std::runtime_error("Unexpected comparison operator");
        }

        if( lastTestResult ) {
            appendInstruction<ins::AndTest>(testResult->id, lastTestResult->id, lastTestResult->id);
        } else {
            lastTestResult = testResult;
        }
    }

    latestObject = lastTestResult;
}

void Compiler::visitName(const ast::Name &name)
{
    lookupObject(name); // sets latest object id
}


void Compiler::visitTypeName(const ast::TypeName &)
{
    throw CompilerBug { "Compiler must not visit type name" };
}

void Compiler::visitTypeParameterList(const ast::TypeParameterList &)
{
    throw MissingFeature("TypeParameterList");
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
    appendInstruction<ins::SetString>(latestObject->id, visitable.mValue);
}

void Compiler::visitType(const ast::Type &visitable)
{
    lookupType(visitable);
}


void Compiler::visitWhile(const ast::While &loop)
{
    const auto ipStartOfCondition = latestInstructionPointer() + 1;

    loop.mCondition->acceptVisitor(*this);
    auto condition = latestObject;
    if( condition->type != BasicType::BOOLEAN ) {
        throw TypeMismatch(loop.position(), "While condition must be boolean"); // TODO: mPosition, text
    }

    appendInstruction<ins::Noop>(); // placeholder for jump_if
    const auto ipJumpIfNot = latestInstructionPointer();

    loop.mBody->acceptVisitor(*this);
    appendInstruction<ins::Jump>(ipStartOfCondition);

    appendInstruction<ins::Noop>(); // Make sure there is something to jump to
    const auto afterLoop = latestInstructionPointer();
    mInstructions[ipJumpIfNot] = std::make_unique<ins::JumpIfNot>(condition->id, afterLoop);
}



void Compiler::visitIfThen(const ast::IfThen &ifThen)
{
    ifThen.mCondition->acceptVisitor(*this);
    auto condition = latestObject;
    if( condition->type != BasicType::BOOLEAN ) {
        throw TypeMismatch(ifThen.mCondition->position(), "If-condition must be boolean");
    }

    appendInstruction<ins::Noop>();
    const auto ipJumpToEnd = latestInstructionPointer();  // Will hold instruction to jump to end

    ifThen.mIfBlock->acceptVisitor(*this);

    appendInstruction<ins::Noop>(); // This is the end
    const auto ipEnd = latestInstructionPointer();

    mInstructions[ipJumpToEnd] = std::make_unique<ins::JumpIfNot>(condition->id, ipEnd);
}

void Compiler::visitIfThenElse(const ast::IfThenElse &ifThenElse)
{
    ifThenElse.mCondition->acceptVisitor(*this);
    const auto condition = latestObject;
    if( condition->type != BasicType::BOOLEAN ) {
        throw TypeMismatch(ifThenElse.mCondition->position(), "If-Else condition must be boolean");
    }

    appendInstruction<ins::Noop>();
    const auto ipJumpToIf = latestInstructionPointer();  // Will hold instruction to jump to if block

    ifThenElse.mElseBlock->acceptVisitor(*this);
    appendInstruction<ins::Noop>();
    const auto ipJumpToEnd = latestInstructionPointer();  // Will hold instruction to jump to end

    appendInstruction<ins::Noop>();
    const auto ipStartIfBlock = latestInstructionPointer();

    ifThenElse.mIfBlock->acceptVisitor(*this);

    // TODO: wrap push_back in method which returns instruction pointer
    appendInstruction<ins::Noop>(); // This is the end
    const auto ipEnd = latestInstructionPointer();

    mInstructions[ipJumpToIf] = std::make_unique<ins::JumpIf>(condition->id, ipStartIfBlock);
    mInstructions[ipJumpToEnd] = std::make_unique<ins::Jump>(ipEnd);
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

    appendInstruction<ins::OrTest>(lhs->id, rhs->id, latestObject->id);
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

    appendInstruction<ins::AndTest>(lhs->id, rhs->id, latestObject->id);
}

void Compiler::loadPrelude()
{
    registerBuiltinFunction<PrintInt>({"print", {}, {BasicType::INT}});
    registerBuiltinFunction<PrintString>({"print", {}, {BasicType::STRING}});

    lookupOrCreate({"stdin"}); // TODO: no need to lookup
    appendInstruction<ins::SetAllocated>(latestObject->id, &std::make_unique<obj::Childless>);
    const auto type = latestObject->type = mTypeCreator.getType({"Stdin"});
    registerBuiltinFunction<NextStdin>({"next", {}, {type}});

    // Register type names
    mLookup.setType("None", BasicType::NONE);
    mLookup.setType("Bool", BasicType::BOOLEAN);
    mLookup.setType("Int", BasicType::INT);
    mLookup.setType("Float", BasicType::FLOAT);
    mLookup.setType("String", BasicType::STRING);

    // TODO: use type generator
    mLookup.setType("List<String>", typeCreator().getType({"List", {BasicType::STRING}}));
    mLookup.setFunction(std::make_unique<ListCtor>());
}


void Compiler::lookupObject(const ast::Name &variable)
{
    if( auto object = mLookup.lookupObject(variable.mName) ) {
        latestObject = object;
    } else {
        throw UndefinedVariable(variable.position(), variable.mName);
    }
}


void Compiler::lookupType(const ast::Type & typeTree)
{
    const auto typeString = typeTree.toString();

    Type typeId;
    try {
        typeId = mLookup.lookupType(typeString);
    } catch(const LookupError &) {

        throw UnknownType(typeTree.position(), typeString);
    }

    latestType = typeId;
}

const Function * Compiler::lookupFunction(const std::string & functionName,
                                          const std::vector<Type> & typeParameters,
                                          const std::vector<Type> & argumentTypes, const Position & position)
{
    const auto key = FunctionKey {functionName, typeParameters, argumentTypes};
    auto * function = mLookup.lookupFunction(key);

    if( ! function ) {

        throw UnknownFunction(position, key.toString());
    }

    return function;
}


bool Compiler::lookupOrCreate(const std::string &key)
{
    if( auto object = mLookup.lookupObject(key) ) {

        latestObject = object;

        return false;

    } else {

        latestObject = mObjectProvider.createObject();
        mLookup.setObject(key, latestObject);

        return true;
    }
}


InstructionPointer Compiler::latestInstructionPointer() const
{
    return mInstructions.size() - 1;
}

} // namespace ct
