#include "compiler.hpp"
#include "parser/ast.hpp"
#include "common/exceptions.hpp"
#include "common/builtins.hpp"

#include <sstream>


Compiler::Compiler()
    :latestObject(mObjectProvider.createObject())
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
    if( lhs.type != ValueType::INT || rhs.type != ValueType::INT) {
        throw TypeMismatch(addition.position(), ""); // TODO: mPosition, text
    }

    latestObject = mObjectProvider.createObject(lhs.type);

    mInstructions.push_back(std::make_unique<instructions::AddInt>(lhs.id, rhs.id, latestObject.id));
}

void Compiler::visitAssignment(const ast::Assignment &assignment)
{
    assignment.mValue->acceptVisitor(*this);
    const auto source = latestObject;

    // TODO: what if assignee is not name?
    auto name = dynamic_cast<ast::Name*>(assignment.mAssignee.get());

    const auto created = lookupOrCreate({name->mName, {}});
    auto destination = latestObject;
    if( ! created && destination.type != source.type ) {
        throw TypeMismatch(assignment.position(), ""); // TODO: mPosition, text
    }
    destination.type = source.type;

    mInstructions.push_back(std::make_unique<instructions::Copy>(source.id, destination.id));
}

void Compiler::visitFunctionCall(const ast::FunctionCall &functionCall)
{
    std::vector<ValueType> argumentTypes;

    std::vector<ObjectId> originalArgumentIds;
    for( const auto & arg : functionCall.mArguments ) {
        arg->acceptVisitor(*this);
        originalArgumentIds.push_back(latestObject.id);
        // FIXME: what if argument is a function?
        argumentTypes.push_back(latestObject.type.returnType);
    }

    lookup(*functionCall.mName, argumentTypes);
    const auto function = latestObject;

    ObjectId firstArg = 0;
    for( const auto originalArgumentId : originalArgumentIds ) {
        const auto argument = mObjectProvider.createObject();
        if(  firstArg == 0 ) firstArg = argument.id;
        // TODO: memory management
        mInstructions.push_back(std::make_unique<instructions::Copy>(originalArgumentId, argument.id));
    }

    auto returnValue = latestObject = mObjectProvider.createObject();

    returnValue.type = {MetaType::Value, function.type.returnType}; // type of function is type of return value

    mInstructions.push_back(std::make_unique<instructions::CallFunction>(function.id, firstArg, returnValue.id));
}

void Compiler::visitIntLiteral(const ast::IntLiteral &literal)
{
    latestObject = mObjectProvider.createObject();
    latestObject.type = {MetaType::Value, ValueType::INT};
    mInstructions.push_back(std::make_unique<instructions::SetInt>(latestObject.id, literal.mValue));
}

void Compiler::visitFloatLiteral(const ast::FloatLiteral &literal)
{
    latestObject = mObjectProvider.createObject();
    latestObject.type = {MetaType::Value, ValueType::FLOAT};
    mInstructions.push_back(std::make_unique<instructions::SetFloat>(latestObject.id, literal.mValue));
}

void Compiler::visitBooleanLiteral(const ast::BooleanLiteral &literal)
{
    latestObject = mObjectProvider.createObject();
    latestObject.type = {MetaType::Value, ValueType::BOOLEAN};
    mInstructions.push_back(std::make_unique<instructions::SetBoolean>(latestObject.id, literal.mValue));
}

void Compiler::visitComparison(const ast::Comparison &visitable)
{
    if( visitable.mOperands.size() < 1 ) {
        throw CompilerBug("Expected at least one comparison");
    }

    std::optional<CompileTimeObject> lastTestResult;

    // TODO: short circuiting
    for( size_t i = 0; i < visitable.mOperators.size(); i++ ) {
        visitable.mOperands.at(i)->acceptVisitor(*this);
        const auto lhs = latestObject;
        visitable.mOperands.at(i+1)->acceptVisitor(*this);
        const auto rhs = latestObject;

        if( lhs.type != rhs.type) {
            throw TypeMismatch(visitable.position(), "Comparison operators must have same type");
        }

        // TODO: allow other types than int
        if( lhs.type != ValueType::INT ) {
            throw TypeMismatch(visitable.position(), "Only integer comparisons are supported right now");
        }

        auto testResult = mObjectProvider.createObject();
        testResult.type = {MetaType::Value, ValueType::BOOLEAN};

        const auto & op = visitable.mOperators.at(i);
        switch (op) {
        case Token::LessThan:
            mInstructions.push_back(std::make_unique<instructions::IntLessThan>(lhs.id, rhs.id, testResult.id));
            break;
        case Token::LTE:
            mInstructions.push_back(std::make_unique<instructions::IntLTE>(lhs.id, rhs.id, testResult.id));
            break;
        case Token::Equal:
            mInstructions.push_back(std::make_unique<instructions::IsEqual>(lhs.id, rhs.id, testResult.id));
            break;
        case Token::NotEqual:
            mInstructions.push_back(std::make_unique<instructions::IsNotEqual>(lhs.id, rhs.id, testResult.id));
            break;
        case Token::GTE:
            mInstructions.push_back(std::make_unique<instructions::IntGTE>(lhs.id, rhs.id, testResult.id));
            break;
        case Token::GreaterThan:
            mInstructions.push_back(std::make_unique<instructions::IntGreaterThan>(lhs.id, rhs.id, testResult.id));
            break;
        default:
            throw std::runtime_error("Unexpected comparison operator");
            break;
        }

        if( lastTestResult ) {
            mInstructions.push_back(std::make_unique<instructions::AndTest>(testResult.id, lastTestResult->id, lastTestResult->id));
        } else {
            lastTestResult = testResult;
        }
    }

    latestObject = *lastTestResult;
}

void Compiler::visitName(const ast::Name &name)
{
    lookup(name, {}); // sets latest object id
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
    const auto ipStartOfCondition = latestInstructionPointer() + 1;

    loop.mCondition->acceptVisitor(*this);
    auto condition = latestObject;
    if( condition.type != ValueType::BOOLEAN ) {
        throw TypeMismatch(loop.position(), "While condition must be boolean"); // TODO: mPosition, text
    }

    mInstructions.push_back(std::make_unique<instructions::Noop>()); // placeholder for jump_if
    const auto ipJumpIfNot = latestInstructionPointer();

    loop.mBody->acceptVisitor(*this);
    mInstructions.push_back(std::make_unique<instructions::Jump>(ipStartOfCondition));

    mInstructions.push_back(std::make_unique<instructions::Noop>()); // Make sure there is something to jump to
    const auto afterLoop = latestInstructionPointer();
    mInstructions[ipJumpIfNot] = std::make_unique<instructions::JumpIfNot>(condition.id, afterLoop);
}



void Compiler::visitIfThen(const ast::IfThen &ifThen)
{
    ifThen.mCondition->acceptVisitor(*this);
    auto condition = latestObject;
    if( condition.type != ValueType::BOOLEAN ) {
        throw TypeMismatch(ifThen.mCondition->position(), "If-condition must be boolean");
    }

    mInstructions.push_back(std::make_unique<instructions::Noop>());
    const auto ipJumpToEnd = latestInstructionPointer();  // Will hold instruction to jump to end

    ifThen.mIfBlock->acceptVisitor(*this);

    mInstructions.push_back(std::make_unique<instructions::Noop>()); // This is the end
    const auto ipEnd = latestInstructionPointer();

    mInstructions[ipJumpToEnd] = std::make_unique<instructions::JumpIfNot>(condition.id, ipEnd);
}

void Compiler::visitIfThenElse(const ast::IfThenElse &ifThenElse)
{
    ifThenElse.mCondition->acceptVisitor(*this);
    const auto condition = latestObject;
    if( condition.type != ValueType::BOOLEAN ) {
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

    mInstructions[ipJumpToIf] = std::make_unique<instructions::JumpIf>(condition.id, ipStartIfBlock);
    mInstructions[ipJumpToEnd] = std::make_unique<instructions::Jump>(ipEnd);
}

void Compiler::visitOr(const ast::Or &test)
{
    test.mLeft->acceptVisitor(*this);
    const auto lhs = latestObject;
    test.mRight->acceptVisitor(*this);
    const auto rhs = latestObject;

    // TODO: other forms off addition
    if( lhs.type != ValueType::BOOLEAN || rhs.type != ValueType::BOOLEAN) {
        throw TypeMismatch(test.position(), "Both operands of 'or' must be boolean");
    }

    latestObject = mObjectProvider.createObject();
    latestObject.type = lhs.type;

    mInstructions.push_back(std::make_unique<instructions::OrTest>(lhs.id, rhs.id, latestObject.id));
}

void Compiler::visitAnd(const ast::And &test) // TODO: unify operators
{
    test.mLeft->acceptVisitor(*this);
    const auto lhs = latestObject;
    test.mRight->acceptVisitor(*this);
    const auto rhs = latestObject;

    // TODO: other forms off addition
    if( lhs.type != ValueType::BOOLEAN || rhs.type != ValueType::BOOLEAN) {
        throw TypeMismatch(test.position(), "Both operands of 'and' must be boolean");
    }

    latestObject = mObjectProvider.createObject();
    latestObject.type = lhs.type;

    mInstructions.push_back(std::make_unique<instructions::AndTest>(lhs.id, rhs.id, latestObject.id));
}

void Compiler::loadPrelude()
{
    registerBuiltinFunction(new PrintInt, "print");
    registerBuiltinFunction(new AddInt, "__add__"); // FIXME: memory management
    registerBuiltinFunction(new Dummy, "dummy");
}

void Compiler::registerBuiltinFunction(obj::Function * func, const std::string &name)
{
    // TODO: no need to lookup
    lookupOrCreate({name, func->argumentTypes()});
    latestObject.type = {MetaType::Value, func->returnType()}; // FIXME: back to simple types
    // FIXME: memory management
    mInstructions.push_back(std::make_unique<instructions::SetFunction>(latestObject.id, func));
}

void Compiler::lookup(const ast::Name &variable, const std::vector<ValueType> &argumentTypes)
{
    try {
        latestObject = mLookup.lookup({variable.mName, argumentTypes});
    } catch(const LookupError &) {

        // TODO: readable argument types
        // TODO: key.is_function
        std::stringstream msg;
        msg << variable.mName;
        if( ! argumentTypes.empty() ) msg << " with function arguments ";
        // FIXME: add type to string
//        for(auto arg : argumentTypes) msg << arg << " ";

        throw UndefinedVariable(variable.position(), msg.str());
    }
}

bool Compiler::lookupOrCreate(const LookupKey &key)
{
    bool created = false;

    try {
        latestObject = mLookup.lookup(key);

        return false;

    } catch(const LookupError &) {
        // TODO: ugly try catch in non-exceptional state
        //       use optional instead
        latestObject = mObjectProvider.createObject();
        mLookup.set(key, latestObject);

        return true;
    }

    return created;
}


InstructionPointer Compiler::latestInstructionPointer() const
{
    return mInstructions.size() - 1;
}
