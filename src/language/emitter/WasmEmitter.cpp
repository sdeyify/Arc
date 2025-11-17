#include "Emitter.h"
#include "../parser/Parser.h"
#include "../util/Util.h"

std::vector<byte> Arc::WasmEmitter::emitBytecode() {
    std::vector<byte> bytecode;
    
    bytecode.insert(bytecode.end(), std::begin(this->magicHeader), std::end(this->magicHeader));
    bytecode.insert(bytecode.end(), std::begin(this->versionNumber), std::end(this->versionNumber));

    return bytecode;
}

std::vector<byte> Arc::WasmEmitter::getStringifiedBitvalue(const std::string& raw, const Arc::IRBitsizeKind& bitsize) {
    switch (bitsize) {
        case Arc::IRBitsizeKind::NativeI1: return this->numToBytes<bool>(raw == "1");
        case Arc::IRBitsizeKind::NativeI8: return this->numToBytes<int8_t>(std::stoll(raw));
        case Arc::IRBitsizeKind::NativeI16: return this->numToBytes<int16_t>(std::stoll(raw));
        case Arc::IRBitsizeKind::NativeI32: return this->numToBytes<int32_t>(std::stoll(raw));
        case Arc::IRBitsizeKind::NativeI64: return this->numToBytes<int64_t>(std::stoll(raw));
        case Arc::IRBitsizeKind::NativeU8: return this->numToBytes<uint8_t>(std::stoull(raw));
        case Arc::IRBitsizeKind::NativeU16: return this->numToBytes<uint16_t>(std::stoull(raw));
        case Arc::IRBitsizeKind::NativeU32: return this->numToBytes<uint32_t>(std::stoull(raw));
        case Arc::IRBitsizeKind::NativeU64: return this->numToBytes<uint64_t>(std::stoull(raw));
        case Arc::IRBitsizeKind::NativeF16: return this->numToBytes<float>(std::stof(raw));
        case Arc::IRBitsizeKind::NativeBF16: return this->numToBytes<float>(std::stof(raw));
        case Arc::IRBitsizeKind::NativeF32: return this->numToBytes<float>(std::stof(raw));
        case Arc::IRBitsizeKind::NativeF64: return this->numToBytes<double>(std::stod(raw));
    }
}

std::vector<byte> Arc::WasmEmitter::parseInstruction(const Arc::Instruction& instruction) {
    switch (instruction->kind()) {
        case Arc::IRInstructionKind::ArrayLength: return {0x25, 0x01, 0xFB, 0x07};
        case Arc::IRInstructionKind::BinaryOperation: return this->parseBinaryOperation(*instruction->cast<Arc::IRNodes::BinaryOperation>());
        case Arc::IRInstructionKind::Bitcast: return this->parseBitcast(*instruction->cast<Arc::IRNodes::Bitcast>());
        case Arc::IRInstructionKind::BranchIf: return {};
        case Arc::IRInstructionKind::Bubble: return this->parseBubble(instruction->cast<Arc::IRNodes::Bubble>()->stackAddress);
        case Arc::IRInstructionKind::Call: return {0x10, this->functionIds[instruction->cast<Arc::IRNodes::Call>()->functionDirectiveId]};
        case Arc::IRInstructionKind::CallIndirect: return {0x25, 0x00, 0xD2};
        case Arc::IRInstructionKind::DeleteObject: return {0xD0, 0x6F, 0x26, 0x02};
        case Arc::IRInstructionKind::Exit: return {0x00};
        case Arc::IRInstructionKind::FreePointer: return {0x41, 0x00, 0x36, 0x02, 0x00};
        case Arc::IRInstructionKind::FunctionPointerOf: return {0x41, this->functionIds[instruction->cast<Arc::IRNodes::FunctionPointerOf>()->functionDirectiveId]};
        case Arc::IRInstructionKind::LoadArray: return {0x25, 0x01, 0xFB, 0x05};
        case Arc::IRInstructionKind::Label: return {};
        case Arc::IRInstructionKind::LoadDirectivePointer: return {0x23, static_cast<byte>(instruction->cast<Arc::IRNodes::LoadDirectivePointer>()->directiveId)};
        case Arc::IRInstructionKind::LoadField: return {0xFC, 0x13, 0x02, 0xFC, 0x03, this->structIds[instruction->cast<Arc::IRNodes::LoadField>()->structId], static_cast<byte>(instruction->cast<Arc::IRNodes::LoadField>()->fieldId)};
        case Arc::IRInstructionKind::LoadLocal: return {0x20, static_cast<byte>(instruction->cast<Arc::IRNodes::LoadLocal>()->localId)};
        case Arc::IRInstructionKind::LoadNull: return {0x41, 0x00};
        case Arc::IRInstructionKind::LoadPointer: return this->parseLoadPointer(instruction->cast<Arc::IRNodes::Pointer>()->bitsize);
        case Arc::IRInstructionKind::LoadValue: return this->parseLoadValue(*instruction->cast<Arc::IRNodes::LoadValue>());
        case Arc::IRInstructionKind::NewArray: return {0x41, 0x01, 0xFC, 0x0F, 0x02, 0x41, 0x01, 0xFC, 0x12, 0x42, 0x26};
        case Arc::IRInstructionKind::NewObject: return this->parseNewObject(*instruction->cast<Arc::IRNodes::NewObject>());
        case Arc::IRInstructionKind::ObjectCast: return {0xFC, 0x16, this->structIds[instruction->cast<Arc::IRNodes::ObjectCast>()->endStructId]};
        case Arc::IRInstructionKind::Pointer: return this->parseStorePointer(instruction->cast<Arc::IRNodes::Pointer>()->bitsize);
        case Arc::IRInstructionKind::Pop: return {0x1A};
        case Arc::IRInstructionKind::Return: return {0x0F};
        case Arc::IRInstructionKind::StoreArray: return {0x25, 0x01, 0xFB, 0x06};
        case Arc::IRInstructionKind::StoreField: return {0xFC, 0x13, 0x02, 0xFC, 0x01, this->structIds[instruction->cast<Arc::IRNodes::StoreField>()->structId], static_cast<byte>(instruction->cast<Arc::IRNodes::StoreField>()->fieldId)};
        case Arc::IRInstructionKind::StoreLocal: return {0x21, static_cast<byte>(instruction->cast<Arc::IRNodes::StoreLocal>()->localId)};
        case Arc::IRInstructionKind::StorePointer: return this->parseStorePointer(instruction->cast<Arc::IRNodes::StorePointer>()->bitsize);
        case Arc::IRInstructionKind::Switch: return {};
        case Arc::IRInstructionKind::UnaryOperation: return this->parseUnaryOperation(*instruction->cast<Arc::IRNodes::UnaryOperation>());
    }
}

byte Arc::WasmEmitter::parseBitsize(const Arc::IRBitsizeKind& bitsize) {
    switch (bitsize) {
        case Arc::IRBitsizeKind::NativeI1:
        case Arc::IRBitsizeKind::NativeI8:
        case Arc::IRBitsizeKind::NativeI16:
        case Arc::IRBitsizeKind::NativeI32:
        case Arc::IRBitsizeKind::NativeU8:
        case Arc::IRBitsizeKind::NativeU16:
        case Arc::IRBitsizeKind::NativeU32:
            return 0x7f;
        case Arc::IRBitsizeKind::NativeI64:
        case Arc::IRBitsizeKind::NativeU64:
            return 0x7E;
        case Arc::IRBitsizeKind::NativeF16:
        case Arc::IRBitsizeKind::NativeBF16:
        case Arc::IRBitsizeKind::NativeF32:
            return 0x7D;
        case Arc::IRBitsizeKind::NativeF64:
            return 0x7C;
    }
}

byte Arc::WasmEmitter::parseLoadBitsize(const Arc::WasmBitsize& bitsize) {
    switch (bitsize) {
        case Arc::WasmBitsize::I32:
            return 0x41;
        case Arc::WasmBitsize::I64:
            return 0x42;
        case Arc::WasmBitsize::F32:
            return 0x43;
        case Arc::WasmBitsize::F64:
            return 0x44;
    }
}

Arc::WasmBitsize Arc::WasmEmitter::getWasmBitsize(const Arc::IRBitsizeKind& bitsize) {
    switch (bitsize) {
        case Arc::IRBitsizeKind::NativeI1:
        case Arc::IRBitsizeKind::NativeI8:
        case Arc::IRBitsizeKind::NativeI16:
        case Arc::IRBitsizeKind::NativeI32:
        case Arc::IRBitsizeKind::NativeU8:
        case Arc::IRBitsizeKind::NativeU16:
        case Arc::IRBitsizeKind::NativeU32:
            return Arc::WasmBitsize::I32;
        case Arc::IRBitsizeKind::NativeI64:
        case Arc::IRBitsizeKind::NativeU64:
            return Arc::WasmBitsize::I64;
        case Arc::IRBitsizeKind::NativeF16:
        case Arc::IRBitsizeKind::NativeBF16:
        case Arc::IRBitsizeKind::NativeF32:
            return Arc::WasmBitsize::F32;
        case Arc::IRBitsizeKind::NativeF64:
            return Arc::WasmBitsize::F64;
    }
}

void Arc::WasmEmitter::parseFunction(const Arc::IRNodes::FunctionDirective& directive) {
    this->functionIds[directive.id] = this->highestFunctionId++;

    this->typeSection.push_back(0x60);

    const Arc::IRNodes::ParameterMetadata parameters = *directive.metadata.expectCast<Arc::IRNodes::ParameterMetadata>(Arc::IRMetadataKind::Parameters, "");

    this->typeSection.push_back(parameters.data.size());

    for (const Arc::IRBitsizeKind& bitsize : parameters.data) {
        this->typeSection.push_back(this->parseBitsize(bitsize));
    }

    this->typeSection.push_back(0x01);

    const Arc::IRNodes::ReturnTypeMetadata returnType = *directive.metadata.expectCast<Arc::IRNodes::ReturnTypeMetadata>(Arc::IRMetadataKind::ReturnType, "");

    this->typeSection.push_back(this->parseBitsize(returnType.data));

    this->functionSection.push_back(this->highestFunctionId - 1);

    const std::vector<Arc::Instruction> body = this->getInstructions(directive.implementationAddress);

    this->codeSection.insert(this->codeSection.end(), body.begin(), body.end());
}

void Arc::WasmEmitter::parseStruct(const Arc::IRNodes::StructDirective& directive) {
    this->typeSection.push_back(0x5F);
    this->typeSection.push_back(directive.fields.size());

    for (const Arc::IRBitsizeKind field : std::views::values(directive.fields)) {
        this->typeSection.push_back(0x00);
        this->typeSection.push_back(this->parseBitsize(field));
    }

    this->structIds[directive.id] = ++this->highestStructId;
}

void Arc::WasmEmitter::parsePointerDirective(const Arc::IRNodes::PointerDirective& directive) {
    this->globalSection.push_back(this->parseBitsize(directive.bitsize));
    this->globalSection.push_back(0x00);

    this->globalSection.push_back(this->parseLoadBitsize(this->getWasmBitsize(directive.bitsize)));

    const std::vector<byte> value = this->getStringifiedBitvalue(directive.rawRepresentation, directive.bitsize);
    this->globalSection.insert(this->globalSection.end(), value.begin(), value.end());

    this->globalIds[directive.id] = this->highestGlobalId++;

    this->globalSection.push_back(0x0B);
}

std::vector<byte> Arc::WasmEmitter::parseBinaryOperation(const Arc::IRNodes::BinaryOperation& instruction) {
    switch (instruction.op) {
        
    }
}

std::vector<byte> Arc::WasmEmitter::parseUnaryOperation(const Arc::IRNodes::UnaryOperation& instruction) {
    const Arc::WasmBitsize wasmBitsize = this->getWasmBitsize(instruction.bitsize);
    const std::vector<byte> i32CastBytes = this->castTable.at(Arc::WasmBitsize::I32).at(Arc::WasmBitsize::I32);
    const std::vector<byte> i64CastBytes = this->castTable.at(Arc::WasmBitsize::I64).at(Arc::WasmBitsize::I64);
    const std::vector<byte> f32CastBytes = this->castTable.at(Arc::WasmBitsize::I32).at(Arc::WasmBitsize::F32);
    const std::vector<byte> f64CastBytes = this->castTable.at(Arc::WasmBitsize::I64).at(Arc::WasmBitsize::F64);

    switch (instruction.op) {
        case Arc::IRUnaryOperationKind::Not: {
            return this->getOnWasmBitsize<std::vector<byte>>(
                wasmBitsize,
                Util::joinvec(f32CastBytes, {0x41, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0x73, 0x45}), /* F32 */
                Util::joinvec(f64CastBytes, {0x42, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x87, 0x50}), /* F64 */
                {0x41, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0x73, 0x45}, /* I32 */
                {0x42, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x87, 0x50} /* I64 */
            );
        }
        case Arc::IRUnaryOperationKind::Negative:
            return this->getOnWasmBitsize<std::vector<byte>>(
                wasmBitsize,
                Util::joinvec(f32CastBytes, {0x41, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0x6C}), /* F32 */
                Util::joinvec(f64CastBytes, {0x42, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0x7F}), /* F64 */
                {0x41, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0x6C}, /* I32 */
                {0x42, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xFF, 0x7F} /* I64 */
            );
        case Arc::IRUnaryOperationKind::Positive:
            return this->getOnWasmBitsize<std::vector<byte>>(
                wasmBitsize,
                Util::joinvec(f32CastBytes, {0x41, 0x00, 0x48, 0x41, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0x6C, 0x1B}), /* F32 */
                Util::joinvec(f64CastBytes, {0x42, 0x00, 0x50, 0x42, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x7E, 0x1B}), /* F64 */
                {0x41, 0x00, 0x48, 0x41, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0x6C, 0x1B}, /* I32 */
                {0x42, 0x00, 0x50, 0x42, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x7E, 0x1B} /* I64 */
            );
            break;
        case Arc::IRUnaryOperationKind::Ceil:
            return this->getOnWasmBitsize<std::vector<byte>>(
                wasmBitsize,
                {0x8D}, /* F32 */
                {0x9B}, /* F64 */
                Util::joinvec(i32CastBytes, {0x8D}), /* I32 */
                Util::joinvec(i32CastBytes, {0x9B}) /* I64 */
            );
            break;
        case Arc::IRUnaryOperationKind::Floor:
            return this->getOnWasmBitsize<std::vector<byte>>(
                wasmBitsize,
                {0x8E}, /* F32 */
                {0x9C}, /* F64 */
                Util::joinvec(i32CastBytes, {0x8E}), /* I32 */
                Util::joinvec(i64CastBytes, {0x9C}) /* I64 */
            );
            break;
        case Arc::IRUnaryOperationKind::CountLeadingZeros:
            return this->getOnWasmBitsize<std::vector<byte>>(
                wasmBitsize,
                Util::joinvec(i32CastBytes, {0x67}), /* F32 */
                Util::joinvec(i64CastBytes, {0x70}), /* F64 */
                {0x67}, /* I32 */
                {0x70} /* I64 */
            );
            break;
        case Arc::IRUnaryOperationKind::CountEndingZeros:
            return this->getOnWasmBitsize<std::vector<byte>>(
                wasmBitsize,
                Util::joinvec(f32CastBytes, {0x68}), /* F32 */
                Util::joinvec(f64CastBytes, {0x7A}), /* F64 */
                {0x68}, /* I32 */
                {0x7A} /* I64 */
            );
            break;
        case Arc::IRUnaryOperationKind::CountLeadingOnes:
            return this->getOnWasmBitsize<std::vector<byte>>(
                wasmBitsize,
                Util::joinvec(f32CastBytes, {0x41, 0xFF, 0xFF, 0xFF, 0xFF, 0x73, 0x67}), /* F32 */
                Util::joinvec(f64CastBytes, {0x42, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x87, 0x79}), /* F64 */
                {0x41, 0xFF, 0xFF, 0xFF, 0xFF, 0x73, 0x67}, /* I32 */
                {0x42, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x87, 0x79} /* I64 */
            );
            break;
        case Arc::IRUnaryOperationKind::CountEndingOnes:
            return this->getOnWasmBitsize<std::vector<byte>>(
                wasmBitsize,
                Util::joinvec(f32CastBytes, {0x41, 0xFF, 0xFF, 0xFF, 0xFF, 0x73, 0x68}), /* F32 */
                Util::joinvec(f64CastBytes, {0x42, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x87, 0x79}), /* F64 */
                {0x41, 0xFF, 0xFF, 0xFF, 0xFF, 0x73, 0x68}, /* I32 */
                {0x42, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x87, 0x79} /* I64 */
            );
            break;
        case Arc::IRUnaryOperationKind::CountOnes:
            return this->getOnWasmBitsize<std::vector<byte>>(
                wasmBitsize,
                Util::joinvec(f32CastBytes, {0x69}), /* F32 */
                Util::joinvec(f64CastBytes, {0x7B}), /* F64 */
                {0x69}, /* I32 */
                {0x7B} /* I64 */
            );
            break;
        case Arc::IRUnaryOperationKind::CountZeros:
            return this->getOnWasmBitsize<std::vector<byte>>(
                wasmBitsize,
                Util::joinvec(f32CastBytes, {0x69, 0x41, 32, 0x6B}), /* F32 */
                Util::joinvec(f64CastBytes, {0x7B, 0x42, 32, 0x7D}), /* F64 */
                {0x69, 0x41, 32, 0x6B}, /* I32 */
                {0x7B, 0x42, 32, 0x7D} /* I64 */
            );
            break;
    }
}

std::vector<byte> Arc::WasmEmitter::parseLoadPointer(const Arc::IRBitsizeKind& bitsize) {
    switch (bitsize) {
        case Arc::IRBitsizeKind::NativeI1: return {0x2D};
        case Arc::IRBitsizeKind::NativeI8: return {0x2C};
        case Arc::IRBitsizeKind::NativeI16: return {0x2E};
        case Arc::IRBitsizeKind::NativeI32: return {0x28};
        case Arc::IRBitsizeKind::NativeI64: return {0x29};
        case Arc::IRBitsizeKind::NativeU8: return {0x2D};
        case Arc::IRBitsizeKind::NativeU16: return {0x2F};
        case Arc::IRBitsizeKind::NativeU32: return {0x35};
        case Arc::IRBitsizeKind::NativeU64: return {0x29};
        case Arc::IRBitsizeKind::NativeF16: return {0x2A};
        case Arc::IRBitsizeKind::NativeBF16: return {0x2A};
        case Arc::IRBitsizeKind::NativeF32: return {0x2B};
        case Arc::IRBitsizeKind::NativeF64: return {0x2B};
    }
}

std::vector<byte> Arc::WasmEmitter::parseStorePointer(const Arc::IRBitsizeKind& bitsize) {
    switch (bitsize) {
        case Arc::IRBitsizeKind::NativeI1: return {0x2E};
        case Arc::IRBitsizeKind::NativeI8: return {0x2E};
        case Arc::IRBitsizeKind::NativeI16: return {0x2E};
        case Arc::IRBitsizeKind::NativeI32: return {0x36};
        case Arc::IRBitsizeKind::NativeI64: return {0x37};
        case Arc::IRBitsizeKind::NativeU8: return {0x3A};
        case Arc::IRBitsizeKind::NativeU16: return {0x3B};
        case Arc::IRBitsizeKind::NativeU32: return {0x3D};
        case Arc::IRBitsizeKind::NativeU64: return {0x3E};
        case Arc::IRBitsizeKind::NativeF16: return {0x38};
        case Arc::IRBitsizeKind::NativeBF16: return {0x38};
        case Arc::IRBitsizeKind::NativeF32: return {0x38};
        case Arc::IRBitsizeKind::NativeF64: return {0x39};
    }
}

std::vector<byte> Arc::WasmEmitter::parsePointer(const Arc::IRNodes::Pointer& instruction) {
    std::vector<byte> bytecode = {};

    bytecode.push_back(0x41);
    bytecode.push_back(++this->highestPointerId);

    const std::vector<byte> swapBytes = {21, 00, 22, 01, 20, 00};
    bytecode.insert(bytecode.end(), swapBytes.begin(), swapBytes.end());

    bytecode.push_back(this->parseStorePointer(instruction.bitsize)[0]);

    bytecode.push_back(0x41);
}

std::vector<byte> Arc::WasmEmitter::parseBitcast(const Arc::IRNodes::Bitcast& instruction) {
    return this->castTable.at(this->getWasmBitsize(instruction.startType)).at(this->getWasmBitsize(instruction.endType));
}

std::vector<byte> Arc::WasmEmitter::parseNewObject(const Arc::IRNodes::NewObject& instruction) {
    std::vector<byte> bytecode;

    if (const std::optional<Arc::IRNodes::StructDirective> possibleStruct = this->ir.searchStructDirective(instruction.structDirectiveId)) {
        const Arc::IRNodes::StructDirective structDirective = *possibleStruct;

        for (const Arc::IRBitsizeKind& field : std::views::values(structDirective.fields)) {
            bytecode.push_back(this->parseLoadBitsize(this->getWasmBitsize(field)));
            bytecode.push_back(0x00);
        }

        bytecode.push_back(0xFC);
        bytecode.push_back(0x00);
        bytecode.push_back(this->structIds[structDirective.id]);

        const std::vector<byte> arrayTableUpdate = {0x41, 0x01, 0xFC, 0x12, 0x02, 0xFC, 0x11, 0x02};
        bytecode.insert(bytecode.end(), arrayTableUpdate.cbegin(), arrayTableUpdate.cend());
    } else {
        // throw err
        std::unreachable();
    }

    return bytecode;
}

std::vector<byte> Arc::WasmEmitter::parseLoadValue(const Arc::IRNodes::LoadValue& instruction) {
    std::vector<byte> loadValueBytecode = {};

    const byte loadBitsize = this->parseLoadBitsize(this->getWasmBitsize(instruction.bitsize));
    const std::vector<byte> bitvalue = this->getStringifiedBitvalue(instruction.rawRepresentation, instruction.bitsize);
    loadValueBytecode.push_back(loadBitsize);
    loadValueBytecode.insert(loadValueBytecode.end(), bitvalue.begin(), bitvalue.end());
}

std::vector<byte> Arc::WasmEmitter::parseBubble(const size_t stackAddress) {

}