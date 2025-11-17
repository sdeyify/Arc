#include "Simulator.h"
#include "ArcSimulator.h"
#include "../parser/Parser.h"
#include "../util/imports.h"
#include "../../../../dependencies/half.hpp"
#include "../../../../dependencies/bfloat16.hpp"

int Arc::ArcSimulator::simulate() {
    if (const std::optional<Arc::IRNodes::FunctionDirective> possibleEntrypoint = this->ir.searchFunctionDirective(0)) {
        Arc::IRNodes::FunctionDirective entrypoint = *possibleEntrypoint;
        std::optional<int> simulationResult = this->simulateIR(entrypoint.implementationAddress);

        std::cout << std::flush;

        return simulationResult.value_or(0);
    } else {
        // throw err
        std::unreachable();
    }
}

void Arc::ArcSimulator::loadLabels() {
    size_t instPtr = 0;

    for (const Arc::Instruction& instruction : this->ir.getInstructions()) {
        if (instruction->kind() == Arc::IRInstructionKind::Label) {
            this->labels[instruction->cast<Arc::IRNodes::Label>()->labelId] = instPtr;
        }

        instPtr++;
    }
}

void Arc::ArcSimulator::addStackLocation(const size_t locationPtr) {
    this->stackLocation.push(locationPtr);
}

size_t Arc::ArcSimulator::removeStackLocation() {
    const size_t topLocation = this->stackLocation.top();
    this->stackLocation.pop();
    return topLocation;
}

void Arc::ArcSimulator::updateStackLocation(const size_t locationPtr) {
    this->stackLocation.top() = locationPtr;
}

std::stack<size_t> Arc::ArcSimulator::getStackLocation() {
    return this->stackLocation;
}

size_t& Arc::ArcSimulator::getTopStackLocation() {
    return this->stackLocation.top();
}

Arc::RuntimeValue Arc::ArcSimulator::popStackValue() {
    const Arc::RuntimeValue stackValue = this->stack.back();
    this->stack.pop_back();
    return stackValue;
}

Arc::RuntimeValue Arc::ArcSimulator::castBitValue(const int64_t value, const Arc::IRBitsizeKind& kind, const Arc::StackValueKind& valueKind) {
    switch (kind) {
        case Arc::IRBitsizeKind::NativeI8: return Arc::RuntimeValue(static_cast<int8_t>(value), valueKind);
        case Arc::IRBitsizeKind::NativeI16: return Arc::RuntimeValue(static_cast<int16_t>(value), valueKind);
        case Arc::IRBitsizeKind::NativeI32: return Arc::RuntimeValue(static_cast<int32_t>(value), valueKind);
        case Arc::IRBitsizeKind::NativeI64: return Arc::RuntimeValue(static_cast<int64_t>(value), valueKind);
        case Arc::IRBitsizeKind::NativeU8: return Arc::RuntimeValue(static_cast<uint8_t>(value), valueKind);
        case Arc::IRBitsizeKind::NativeU16: return Arc::RuntimeValue(static_cast<uint16_t>(value), valueKind);
        case Arc::IRBitsizeKind::NativeU32: return Arc::RuntimeValue(static_cast<uint32_t>(value), valueKind);
        case Arc::IRBitsizeKind::NativeU64: return Arc::RuntimeValue(static_cast<uint64_t>(value), valueKind);
        case Arc::IRBitsizeKind::NativeF16: return Arc::RuntimeValue(half_float::half(value), valueKind);
        case Arc::IRBitsizeKind::NativeBF16: return Arc::RuntimeValue(bfloat16(value), valueKind);
        case Arc::IRBitsizeKind::NativeF32: return Arc::RuntimeValue(static_cast<float>(value), valueKind);
        case Arc::IRBitsizeKind::NativeF64: return Arc::RuntimeValue(static_cast<double>(value), valueKind);
    }
}

Arc::RuntimeValue Arc::ArcSimulator::interpretRuntimeValue(const Arc::RuntimeValue& value, const std::optional<Arc::IRBitsizeKind>& bitsize, const Arc::StackValueKind& valueKind) {
    return (bitsize)
        ? this->castBitValue(std::visit<std::int64_t>([](const auto&& value) { return static_cast<std::int64_t>(value); }, value.value), *bitsize, valueKind)
        : value;
}

std::optional<int> Arc::ArcSimulator::simulateIR(const std::size_t locationPtr) {
    const std::vector<Arc::Instruction> instructions = this->ir.getInstructions();

    this->addStackLocation(locationPtr);

    while (this->getTopStackLocation() < instructions.size()) {
        const Arc::InstructionResult& instructionResult = this->simulateInstruction(instructions[this->getTopStackLocation()]);

        switch (instructionResult.kind) {
            case Arc::InstructionResultKind::OrdinaryResult: continue;
            case Arc::InstructionResultKind::ProgramExitResult: return std::make_optional(*instructionResult.possibleExitCode);
            case Arc::InstructionResultKind::ReturnResult: return std::nullopt;
        }

        this->updateStackLocation(++this->getTopStackLocation());
    }

    this->removeStackLocation();

    return std::nullopt;
}

void Arc::ArcSimulator::pushStringifiedBitvalue(const std::string& rawRepresentation, const Arc::IRBitsizeKind& bitsize, const StackValueKind& valueKind) {
    switch (bitsize) {
        case Arc::IRBitsizeKind::NativeI1: this->stack.push_back(Arc::RuntimeValue(rawRepresentation == "1", valueKind)); break;
        case Arc::IRBitsizeKind::NativeI8: this->stack.push_back(Arc::RuntimeValue(static_cast<int8_t>(std::stoll(rawRepresentation)), valueKind)); break;
        case Arc::IRBitsizeKind::NativeI16: this->stack.push_back(Arc::RuntimeValue(static_cast<int16_t>(std::stoll(rawRepresentation)), valueKind)); break;
        case Arc::IRBitsizeKind::NativeI32: this->stack.push_back(Arc::RuntimeValue(static_cast<int32_t>(std::stoll(rawRepresentation)), valueKind)); break;
        case Arc::IRBitsizeKind::NativeI64: this->stack.push_back(Arc::RuntimeValue(static_cast<int64_t>(std::stoll(rawRepresentation)), valueKind)); break;
        case Arc::IRBitsizeKind::NativeU8: this->stack.push_back(Arc::RuntimeValue(static_cast<uint8_t>(std::stoull(rawRepresentation)), valueKind)); break;
        case Arc::IRBitsizeKind::NativeU16: this->stack.push_back(Arc::RuntimeValue(static_cast<uint16_t>(std::stoull(rawRepresentation)), valueKind)); break;
        case Arc::IRBitsizeKind::NativeU32: this->stack.push_back(Arc::RuntimeValue(static_cast<uint32_t>(std::stoull(rawRepresentation)), valueKind)); break;
        case Arc::IRBitsizeKind::NativeU64: this->stack.push_back(Arc::RuntimeValue(static_cast<uint64_t>(std::stoull(rawRepresentation)), valueKind)); break;
        case Arc::IRBitsizeKind::NativeF16: this->stack.push_back(Arc::RuntimeValue(half_float::half(std::stof(rawRepresentation)), valueKind)); break;
        case Arc::IRBitsizeKind::NativeBF16: this->stack.push_back(Arc::RuntimeValue(bfloat16(std::stof(rawRepresentation)), valueKind)); break;
        case Arc::IRBitsizeKind::NativeF32: this->stack.push_back(Arc::RuntimeValue(std::stof(rawRepresentation), valueKind)); break;
        case Arc::IRBitsizeKind::NativeF64: this->stack.push_back(Arc::RuntimeValue(std::stod(rawRepresentation), valueKind)); break;
    }
}

Arc::InstructionResult Arc::ArcSimulator::simulateInstruction(const Arc::Instruction& instruction) {
    switch (instruction->kind()) {
        case Arc::IRInstructionKind::ArrayLength: this->parseArrayLength(); break;
        case Arc::IRInstructionKind::BinaryOperation: this->parseBinaryOperation(*instruction->cast<Arc::IRNodes::BinaryOperation>()); break;
        case Arc::IRInstructionKind::Bitcast: this->parseBitcast(instruction->cast<Arc::IRNodes::Bitcast>()->endType); break;
        case Arc::IRInstructionKind::BranchIf: this->parseBranchIf(instruction->cast<Arc::IRNodes::BranchIf>()->labelId); break;
        case Arc::IRInstructionKind::Breakpoint: this->parseBreakpoint(*instruction->cast<Arc::IRNodes::Breakpoint>()); break;
        case Arc::IRInstructionKind::Bubble: this->parseBubble(instruction->cast<Arc::IRNodes::Bubble>()->stackAddress); break;
        case Arc::IRInstructionKind::Call: this->parseCall(instruction->cast<Arc::IRNodes::Call>()->functionDirectiveId); break;
        case Arc::IRInstructionKind::CallIndirect: this->parseCallIndirect(); break;
        case Arc::IRInstructionKind::DebugWrite: std::cout << instruction->cast<Arc::IRNodes::DebugWrite>()->message; break;
        case Arc::IRInstructionKind::DeleteObject: this->parseDeleteObject(); break;
        case Arc::IRInstructionKind::FreePointer: this->parseFreePointer(); break;
        case Arc::IRInstructionKind::FunctionPointerOf: this->parseFunctionPointerOf(instruction->cast<Arc::IRNodes::FunctionPointerOf>()->functionDirectiveId); break;
        case Arc::IRInstructionKind::Exit: return Arc::InstructionResult::createExit(instruction->cast<Arc::IRNodes::Exit>()->exitCode);
        case Arc::IRInstructionKind::LoadArray: this->parseLoadArray(instruction->cast<Arc::IRNodes::LoadArray>()->bitsize); break;
        case Arc::IRInstructionKind::LoadDirectivePointer: this->parseLoadDirectivePointer(*instruction->cast<Arc::IRNodes::LoadDirectivePointer>()); break;
        case Arc::IRInstructionKind::LoadField: this->parseLoadField(*instruction->cast<Arc::IRNodes::LoadField>()); break;
        case Arc::IRInstructionKind::LoadLocal: this->parseLoadLocal(*instruction->cast<Arc::IRNodes::LoadLocal>()); break;
        case Arc::IRInstructionKind::LoadNull: this->stack.push_back(Arc::RuntimeValue(0, Arc::StackValueKind::Null)); break;
        case Arc::IRInstructionKind::LoadPointer: this->parseLoadPointer(instruction->cast<Arc::IRNodes::LoadPointer>()->bitsize); break;
        case Arc::IRInstructionKind::LoadValue: this->parseLoadValue(*instruction->cast<Arc::IRNodes::LoadValue>()); break;
        case Arc::IRInstructionKind::LoadVoid: this->stack.push_back(Arc::RuntimeValue(std::monostate(), Arc::StackValueKind::Void)); break;
        case Arc::IRInstructionKind::NewArray: this->parseNewArray(instruction->cast<Arc::IRNodes::NewArray>()->bitsize); break;
        case Arc::IRInstructionKind::NewObject: this->parseNewObject(instruction->cast<Arc::IRNodes::NewObject>()->structDirectiveId); break;
        case Arc::IRInstructionKind::ObjectCast: this->parseObjectCast(instruction->cast<Arc::IRNodes::ObjectCast>()->endStructId); break;
        case Arc::IRInstructionKind::Pointer: this->parsePointer(instruction->cast<Arc::IRNodes::Pointer>()->bitsize); break;
        case Arc::IRInstructionKind::Pop: this->stack.pop_back(); break;
        case Arc::IRInstructionKind::Return: return Arc::InstructionResult::createReturn(); break;
        case Arc::IRInstructionKind::StoreArray: this->parseStoreArray(); break;
        case Arc::IRInstructionKind::StoreField: this->parseStoreField(*instruction->cast<Arc::IRNodes::StoreField>()); break;
        case Arc::IRInstructionKind::StoreLocal: this->parseLoadLocal(*instruction->cast<Arc::IRNodes::LoadLocal>()); break;
        case Arc::IRInstructionKind::StorePointer: this->parseStorePointer(*instruction->cast<Arc::IRNodes::StorePointer>()); break;
        case Arc::IRInstructionKind::UnaryOperation: this->parseUnaryOperation(*instruction->cast<Arc::IRNodes::UnaryOperation>()); break;
        case Arc::IRInstructionKind::Switch: this->parseSwitch(instruction->cast<Arc::IRNodes::Switch>()->switchLabelIds); break;
    }

    return Arc::InstructionResult::createOrdinary();
}

void Arc::ArcSimulator::parseBranchIf(const size_t labelId) {
    const bool stackValue = this->getStackValue<bool>();
    
    if (stackValue) {
        this->updateStackLocation(labelId);
    }
}

void Arc::ArcSimulator::parseUnaryOperation(const Arc::IRNodes::UnaryOperation& instruction) {
    const Arc::RuntimeValue value = this->popStackValue();
    auto interpretedValue = this->interpretStackValue(value);

    decltype(interpretedValue) result;

    switch (instruction.op) {
        case Arc::IRUnaryOperationKind::Not: result = !interpretedValue; break;
        case Arc::IRUnaryOperationKind::Negative: result = -interpretedValue; break;
        case Arc::IRUnaryOperationKind::Positive: result = abs(interpretedValue); break;
        case Arc::IRUnaryOperationKind::Ceil: result = ceil(interpretedValue); break;
        case Arc::IRUnaryOperationKind::Floor: result = abs(interpretedValue); break;
        case Arc::IRUnaryOperationKind::CountLeadingZeros: result = std::countl_zero(static_cast<uintmax_t>(interpretedValue)); break;
        case Arc::IRUnaryOperationKind::CountEndingZeros: result = std::countr_zero(static_cast<uintmax_t>(interpretedValue)); break;
        case Arc::IRUnaryOperationKind::CountLeadingOnes: result = std::countl_one(static_cast<uintmax_t>(interpretedValue)); break;
        case Arc::IRUnaryOperationKind::CountEndingOnes: result = std::countr_one(static_cast<uintmax_t>(interpretedValue)); break;
        case Arc::IRUnaryOperationKind::CountOnes: result = std::popcount(static_cast<uintmax_t>(interpretedValue)); break;
        case Arc::IRUnaryOperationKind::CountZeros: result = (8 * sizeof(interpretedValue) - std::popcount(static_cast<uintmax_t>(interpretedValue))); break;
    }

    this->stack.push_back(this->interpretRuntimeValue(Arc::RuntimeValue(result, value.valueKind), std::make_optional(instruction.bitsize), value.valueKind));
}

std::string Arc::ArcSimulator::inspectValue(const Arc::RuntimeValue& value) {
    switch (this->stack.back().valueKind) {
        case Arc::StackValueKind::Null: return "<null>";
        case Arc::StackValueKind::Bitvalue: return std::to_string(this->interpretStackValue(value));
        case Arc::StackValueKind::Array: {
            std::string output;
            const size_t arrayId = this->interpretStackValue<size_t>(this->stack.back());
            output += "array: " + Arc::stringifyBitsize(this->arrayData[arrayId].bitsize) + "[" + std::to_string(this->arrayData[arrayId].length) + "]" + "of: ";

            for (size_t i = 0; i < this->arrayData[arrayId].length; i++) {
                output += this->inspectValue(this->pointers[this->arrayData[arrayId].pointerIdx + i]) + "\n";
            }

            return output;
        }
        case Arc::StackValueKind::Pointer: return "pointer to: " + this->inspectValue(this->pointers[this->interpretStackValue<size_t>(this->stack.back())]);
        case Arc::StackValueKind::Object: {
            std::string output;
            const size_t objectId = this->interpretStackValue<size_t>(this->stack.back());

            output += "object of type: " + (this->environment.structNames.contains(this->heap[objectId].structId))
                ? this->environment.structNames.at(this->heap[objectId].structId)
                : "<unknown>";

            return output;
        }
        case Arc::StackValueKind::Function: return "function " + this->environment.functionNames.at(this->interpretStackValue<size_t>(this->stack.back()));
        default: return "<void>";
    }
}

void Arc::ArcSimulator::evaluateCode(const std::string& src) {
    Arc::Lexer lexer(src);
    Arc::TokenStream tokenStream = lexer.parse();
    Arc::ASTBuilder astBuilder;
    Arc::AbstractSyntaxTree ast = astBuilder.parse(tokenStream, this->environment.path);
    Arc::IRGenerator irGenerator;
    Arc::IRProgram ir = irGenerator.generateIR(ast);

    Arc::ArcSimulator simulator(ir, this->environment);

    simulator.arrayData = this->arrayData;
    simulator.heap = this->heap;
    simulator.labels = this->labels;
    simulator.locals = this->locals;
    simulator.pointers = this->pointers;
    simulator.stack = this->stack;
    
    simulator.simulate();

    this->arrayData = simulator.arrayData;
    this->heap = simulator.heap;
    this->labels = simulator.labels;
    this->locals = simulator.locals;
    this->pointers = simulator.pointers;
    this->stack = simulator.stack;
}

void Arc::ArcSimulator::parseBreakpoint(const Arc::IRNodes::Breakpoint& instruction) {
    switch (this->environment.breakpointEnvironment) {
        case Arc::BreakpointEnvironment::Ordinary: std::cout << "Breakpoints are only allowed in debug mode and in the Arc REPL." << std::endl; break;
        case Arc::BreakpointEnvironment::Debug: this->parseDebugBreakpoint(); break;
        case Arc::BreakpointEnvironment::Repl: this->parseReplBreakpoint(); break;
    }
}

void Arc::ArcSimulator::parseDebugBreakpoint() {
    this->debugger.welcomeMessage();
    this->debugger.debug(this->getTopStackLocation());
}

void Arc::ArcSimulator::parseReplBreakpoint() {
    if (!this->displayedBreakpointMessage) {
        std::cout << "---- Arc REPL ----" << "\n";
        std::cout << "Press [ ENTER ] two times to execute code." << "\n";
        std::cout << "Press [ ENTER ] two times without any code to exit." << "\n";

        this->displayedBreakpointMessage = true;
    }

    const std::string input = this->replReadUserInput();
    this->evaluateCode(input);
    std::cout << this->inspectValue(this->stack.back()) << std::endl;
}

std::string Arc::ArcSimulator::replReadUserInput() const {
    std::string line;
    std::string input;

    while (std::getline(std::cin, line)) {
        if (line.empty()) break;
        input += line + "\n";
    }

    return input;
}

void Arc::ArcSimulator::parseBubble(const size_t stackAddress) {
    const Arc::RuntimeValue value = this->stack[stackAddress];
    this->stack.erase(this->stack.begin() + stackAddress);
    this->stack.push_back(value);
}

void Arc::ArcSimulator::parseLoadLocal(const Arc::IRNodes::LoadLocal& instruction) {
    if (!this->locals.contains(instruction.localId)) {
        // throw err
        std::unreachable();
    } else {
        this->stack.push_back(this->locals[instruction.localId]);
    }
}

void Arc::ArcSimulator::parseStoreLocal(const Arc::IRNodes::StoreLocal& instruction) {
    if (!this->locals.contains(instruction.localId)) {
        // throw err
        std::unreachable();
    } else {
        this->locals[instruction.localId] = this->popStackValue();
    }
}

void Arc::ArcSimulator::parseLoadValue(const Arc::IRNodes::LoadValue& instruction) {
    this->pushStringifiedBitvalue(instruction.rawRepresentation, instruction.bitsize, Arc::StackValueKind::Bitvalue);
}

void Arc::ArcSimulator::parseBinaryOperation(const Arc::IRNodes::BinaryOperation& instruction) {
    const Arc::RuntimeValue leftValue = this->popStackValue();
    const Arc::RuntimeValue rightValue = this->popStackValue();
    const auto interpretedLeft = this->interpretStackValue(leftValue);
    const auto interpretedRight = this->interpretStackValue(rightValue);

    std::common_type_t<decltype(interpretedLeft), decltype(interpretedRight)> result;

    switch (instruction.op) {
        case Arc::IRBinaryOperationKind::Equal: result = interpretedLeft == interpretedRight; break;
        case Arc::IRBinaryOperationKind::NotEqual: result = interpretedLeft != interpretedRight; break;
        case Arc::IRBinaryOperationKind::Modulus: result = interpretedLeft % interpretedRight; break;
        case Arc::IRBinaryOperationKind::Multiply: result = interpretedLeft * interpretedRight; break;
        case Arc::IRBinaryOperationKind::Add: result = interpretedLeft + interpretedRight; break;
        case Arc::IRBinaryOperationKind::Subtract: result = interpretedLeft - interpretedRight; break;
        case Arc::IRBinaryOperationKind::Divide: result = interpretedLeft / interpretedRight; break;
        case Arc::IRBinaryOperationKind::LessThan: result = interpretedLeft < interpretedRight; break;
        case Arc::IRBinaryOperationKind::LessOrEqual: result = interpretedLeft <= interpretedRight; break;
        case Arc::IRBinaryOperationKind::GreaterThan: result = interpretedLeft > interpretedRight; break;
        case Arc::IRBinaryOperationKind::GreaterOrEqual: result = interpretedLeft >= interpretedRight; break;
        case Arc::IRBinaryOperationKind::LeftShift: result = interpretedLeft << interpretedRight; break;
        case Arc::IRBinaryOperationKind::RightShift: result = interpretedLeft >> interpretedRight; break;
        case Arc::IRBinaryOperationKind::BitwiseAnd: result = interpretedLeft & interpretedRight; break;
        case Arc::IRBinaryOperationKind::LogicalAnd: result = static_cast<bool>(interpretedLeft) && static_cast<bool>(interpretedRight); break;
        case Arc::IRBinaryOperationKind::BitwiseOr: result = interpretedLeft | interpretedRight; break;
        case Arc::IRBinaryOperationKind::LogicalOr: result = static_cast<bool>(interpretedLeft) || static_cast<bool>(interpretedRight); break;
        case Arc::IRBinaryOperationKind::BitwiseXOR: result = interpretedLeft ^ interpretedRight; break;
        case Arc::IRBinaryOperationKind::InstanceOf:
            result = (this->heap.size() < interpretedLeft && this->ir.searchStructDirective(interpretedRight))
                ? this->heap[interpretedLeft].structId == interpretedRight
                : 0;
            break;
    }

    this->stack.push_back(this->interpretRuntimeValue(Arc::RuntimeValue(result, leftValue.valueKind), instruction.bitsize, leftValue.valueKind));
}

void Arc::ArcSimulator::parsePointer(const Arc::IRBitsizeKind& bitsize) {
    const Arc::RuntimeValue pointerStackValue = this->popStackValue();
    const Arc::RuntimeValue pointerValue = this->castBitValue(this->interpretStackValue<intmax_t>(pointerStackValue), bitsize, pointerStackValue.valueKind);
    this->pointers.push_back(pointerValue);
    this->stack.push_back(Arc::RuntimeValue(this->pointers.size() -1, Arc::StackValueKind::Pointer));
}

void Arc::ArcSimulator::parseLoadPointer(const std::optional<Arc::IRBitsizeKind>& bitsize) {
    const std::int64_t address = this->getStackValue();

    if (address > this->pointers.size()) {
        this->stack.push_back(Arc::RuntimeValue(0, Arc::StackValueKind::Void));
        // throw err
        return;
    }

    const Arc::RuntimeValue pointerValue = this->pointers[address];

    this->stack.push_back(this->interpretRuntimeValue(pointerValue, bitsize, pointerValue.valueKind));
}

void Arc::ArcSimulator::parseStorePointer(const Arc::IRNodes::StorePointer& instruction) {
    const std::int64_t address = this->getStackValue();
    const Arc::RuntimeValue pointerStackValue = this->popStackValue();
    const Arc::RuntimeValue pointerValue = this->castBitValue(this->interpretStackValue(pointerStackValue), instruction.bitsize, pointerStackValue.valueKind);

    if (address + instruction.offset > this->pointers.size()) {
        // throw err
        std::unreachable();
        return;
    }

    this->pointers[address + instruction.offset] = pointerValue;
}

void Arc::ArcSimulator::parseFreePointer() {
    const size_t pointerId = this->getStackValue<size_t>();
    this->pointers.erase(this->pointers.begin() + pointerId);
}

void Arc::ArcSimulator::parseArrayLength() {
    const std::int64_t arrayId = this->getStackValue();

    if (arrayId > this->arrayData.size()) {
        // throw err
        std::unreachable();
    }

    this->stack.push_back(Arc::RuntimeValue(this->arrayData[arrayId].length, Arc::StackValueKind::Bitvalue));
}

void Arc::ArcSimulator::parseLoadArray(const std::optional<Arc::IRBitsizeKind>& bitsize) {
    const std::int64_t arrayId = this->getStackValue();
    const std::int64_t loadIdx = this->getStackValue();
    const Arc::ArrayData arrData = this->arrayData[arrayId];
    
    const Arc::RuntimeValue arrayElement = this->pointers[arrData.pointerIdx + loadIdx];

    this->stack.push_back(this->interpretRuntimeValue(arrayElement, bitsize, arrayElement.valueKind));
}

void Arc::ArcSimulator::parseStoreArray() {
    const std::int64_t arrayId = this->getStackValue();
    const std::int64_t storeIdx = this->getStackValue();
    const Arc::RuntimeValue storeValue = this->popStackValue();
    const Arc::ArrayData arrData = this->arrayData[arrayId];
    
    this->pointers[arrData.pointerIdx + storeIdx] = this->interpretRuntimeValue(storeValue, std::make_optional(arrData.bitsize), storeValue.valueKind);
}

void Arc::ArcSimulator::parseNewArray(const Arc::IRBitsizeKind& bitsize) {
    const size_t pointerIdx = this->pointers.size();
    const size_t arrayLength = this->getStackValue<size_t>();

    this->arrayData.push_back(ArrayData(pointerIdx, arrayLength, bitsize));

    this->stack.push_back(Arc::RuntimeValue(this->arrayData.size() - 1, Arc::StackValueKind::Array));

    for (size_t i = 0; i < arrayLength; i++) {
        this->pointers.push_back(this->interpretRuntimeValue(Arc::RuntimeValue(0, Arc::StackValueKind::Bitvalue), std::make_optional(bitsize), Arc::StackValueKind::Bitvalue));
    }
}

void Arc::ArcSimulator::parseFunctionPointerOf(const size_t functionId) {
    this->stack.push_back(Arc::RuntimeValue(functionId, Arc::StackValueKind::Function));
}

void Arc::ArcSimulator::parseCall(const size_t functionId) {
    if (const std::optional<Arc::IRNodes::FunctionDirective> possibleFunction = this->ir.searchFunctionDirective(functionId)) {
        const Arc::IRNodes::FunctionDirective function = *possibleFunction;

        this->updateStackLocation(function.implementationAddress);
    } else {
        std::unreachable(); // throw err
    }
}

void Arc::ArcSimulator::parseCallIndirect() {
    this->parseCall(this->getStackValue<size_t>());
}

void Arc::ArcSimulator::parseLoadDirectivePointer(const Arc::IRNodes::LoadDirectivePointer& instruction) {
    if (const std::optional<Arc::IRNodes::PointerDirective> possiblePointer = this->ir.searchPointerDirective(instruction.directiveId)) {
        if (!this->environment.pointerDirectiveValueKinds.contains(instruction.directiveId)) {
            // throw err
            std::unreachable();
        }

        const Arc::IRNodes::PointerDirective pointer = *possiblePointer;

        this->pushStringifiedBitvalue(pointer.rawRepresentation, instruction.bitsize, this->environment.pointerDirectiveValueKinds.at(instruction.directiveId));
    } else {
        std::unreachable(); // throw err
    }
}

void Arc::ArcSimulator::parseSwitch(const std::vector<size_t>& switchLabelIds) {
    std::unordered_map<size_t, size_t> switchTable;

    for (const size_t& switchLabelId : switchLabelIds) {
        switchTable[runtime_value_hash::hash(this->popStackValue())] = switchLabelId;
    }

    this->updateStackLocation(switchTable[this->getStackValue<size_t>()]);
}

void Arc::ArcSimulator::parseNewObject(const size_t structId) {
    if (const std::optional<Arc::IRNodes::StructDirective> possibleStructDirective = this->ir.searchStructDirective(structId)) {
        const Arc::IRNodes::StructDirective structDirective = *possibleStructDirective;

        std::map<int64_t, Arc::RuntimeValue> objectFields;
        
        for (const int64_t fieldId : std::views::keys(structDirective.fields)) {
            objectFields[fieldId] = this->interpretRuntimeValue(Arc::RuntimeValue(0, structDirective.fieldValueKinds.at(fieldId)), std::make_optional(structDirective.fields.at(fieldId)), structDirective.fieldValueKinds.at(fieldId));
        }

        const Arc::IRNodes::VirtualPointerMetadata vptr = *structDirective.metadata.expectCast<Arc::IRNodes::VirtualPointerMetadata>(Arc::IRMetadataKind::VirtualPointer, "");

        this->heap.push_back(Arc::HeapObject(vptr.data, objectFields, structId));

        this->stack.push_back(Arc::RuntimeValue(this->heap.size() - 1, Arc::StackValueKind::Object));
    } else {
        // throw err
        std::unreachable();
    }
}

void Arc::ArcSimulator::parseDeleteObject() {
    const size_t objectId = this->getStackValue<size_t>();
    this->heap.erase(this->heap.begin() + objectId);
}

void Arc::ArcSimulator::parseLoadField(const Arc::IRNodes::LoadField& instruction) {
    const size_t objectId = this->getStackValue<size_t>();
    Arc::HeapObject object = this->heap[objectId];

    this->stack.push_back(this->interpretRuntimeValue(object.vtable[static_cast<int64_t>(object.vptr + instruction.fieldId)], instruction.bitsize, object.vtable[static_cast<int64_t>(object.vptr + instruction.fieldId)].valueKind));
}

void Arc::ArcSimulator::parseStoreField(const Arc::IRNodes::StoreField& instruction) {
    const size_t objectId = this->getStackValue<size_t>();
    const Arc::RuntimeValue storeValue = this->popStackValue();
    Arc::HeapObject object = this->heap[objectId];

    object.vtable[static_cast<int64_t>(object.vptr + instruction.fieldId)] = this->interpretRuntimeValue(storeValue, instruction.bitsize, storeValue.valueKind);
}

void Arc::ArcSimulator::parseBitcast(const Arc::IRBitsizeKind& endBitsize) {
    const Arc::RuntimeValue bitvalue = this->popStackValue();
    this->stack.push_back(this->interpretRuntimeValue(bitvalue, endBitsize, bitvalue.valueKind));
}

void Arc::ArcSimulator::parseObjectCast(const size_t endStructId) {
    if (const std::optional<Arc::IRNodes::StructDirective> possibleEndStruct = this->ir.searchStructDirective(endStructId)) {
        const Arc::IRNodes::StructDirective endStruct = *possibleEndStruct;
        const size_t objectId = this->getStackValue<size_t>();
        Arc::HeapObject object = this->heap[objectId];

        const Arc::IRNodes::VirtualPointerMetadata vptr = *endStruct.metadata.expectCast<Arc::IRNodes::VirtualPointerMetadata>(Arc::IRMetadataKind::VirtualPointer, "");

        object.structId = endStructId;
        object.vptr = vptr.data;
    } else {
        // throw err
        std::unreachable();
    }
}