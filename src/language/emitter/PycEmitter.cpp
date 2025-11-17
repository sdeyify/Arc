#include "Emitter.h"
#include "../parser/Parser.h"
#include "../util/Util.h"

std::vector<byte> Arc::PycEmitter::getMagicNumber(const byte version) {
    int magicNumber = 2900 + 50 * version;

    const size_t byteCount = sizeof(magicNumber);
    std::vector<std::uint8_t> bytes(byteCount);
    std::memcpy(bytes.data(), &magicNumber, byteCount);

    return bytes;
}

std::vector<byte> Arc::PycEmitter::getTimestamp(const std::filesystem::path& filepath) {
    const std::filesystem::file_time_type time = std::filesystem::last_write_time(filepath);

    const std::chrono::system_clock::time_point systemTime  = std::chrono::system_clock::now();
    const std::chrono::time_point<std::chrono::file_clock> fileTime = std::filesystem::file_time_type::clock::now();

    std::chrono::system_clock::time_point actualTime = systemTime + (time - fileTime);

    const int64_t epochTimestamp = std::chrono::duration_cast<std::chrono::seconds>(actualTime.time_since_epoch()).count();

    const size_t byteCount = sizeof(epochTimestamp);
    std::vector<std::uint8_t> bytes(byteCount);
    std::memcpy(bytes.data(), &epochTimestamp, byteCount);

    return bytes;
}

std::vector<byte> Arc::PycEmitter::getStringifiedBitvalue(const std::string& raw, const Arc::IRBitsizeKind& bitsize) {
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

std::vector<byte> Arc::PycEmitter::parseInstruction(const Arc::Instruction& instruction) {
    switch (instruction->kind()) {
        case Arc::IRInstructionKind::ArrayLength: return {};
        case Arc::IRInstructionKind::BinaryOperation: return {};
        case Arc::IRInstructionKind::Bitcast: return {};
        case Arc::IRInstructionKind::BranchIf: return {};
        case Arc::IRInstructionKind::Breakpoint: return {0x09, 0x00};
        case Arc::IRInstructionKind::Bubble: return {};
        case Arc::IRInstructionKind::Call: return {};
        case Arc::IRInstructionKind::CallIndirect: return {};
        case Arc::IRInstructionKind::DebugWrite: return {0x09, 0x00};
        case Arc::IRInstructionKind::DeleteObject: return {};
        case Arc::IRInstructionKind::Exit: return {0x64, 0x00, 0x5A, 0x00, 0x64, 0x01, 0x6C, 0x00, 0x5A, 0x01, 0x7C, 0x01, 0x63, 0x01, 0x7C, 0x00, 0x5D, 0x01, 0x5E, 0x01, 0x01, 0x00, 0x64, 0x01, 0x53, 0x00};
        case Arc::IRInstructionKind::FreePointer: return {};
        case Arc::IRInstructionKind::FunctionPointerOf: return {};
        case Arc::IRInstructionKind::LoadArray: return {};
        case Arc::IRInstructionKind::Label: return {};
        case Arc::IRInstructionKind::LoadDirectivePointer: return {};
        case Arc::IRInstructionKind::LoadField: return {};
        case Arc::IRInstructionKind::LoadLocal: return {};
        case Arc::IRInstructionKind::LoadNull: return {0x02, 0x00};
        case Arc::IRInstructionKind::LoadPointer: return {};
        case Arc::IRInstructionKind::LoadValue: return this->parseLoadValue(*instruction->cast<Arc::IRNodes::LoadValue>());
        case Arc::IRInstructionKind::LoadVoid: return {0x02, 0x00};
        case Arc::IRInstructionKind::NewArray: return {};
        case Arc::IRInstructionKind::NewObject: return {};
        case Arc::IRInstructionKind::ObjectCast: return {};
        case Arc::IRInstructionKind::Pointer: return {};
        case Arc::IRInstructionKind::Pop: return {0x01, 0x00};
        case Arc::IRInstructionKind::Return: return {0x53, 0x00};
        case Arc::IRInstructionKind::StoreArray: return {};
        case Arc::IRInstructionKind::StoreField: return {};
        case Arc::IRInstructionKind::StoreLocal: return {};
        case Arc::IRInstructionKind::StorePointer: return {};
        case Arc::IRInstructionKind::Switch: return {};
        case Arc::IRInstructionKind::UnaryOperation: return {};
    }
}

std::vector<byte> Arc::PycEmitter::parseLoadValue(const Arc::IRNodes::LoadValue& instruction) {
    this->currentCodeObject.constants.push_back(this->getStringifiedBitvalue(instruction.rawRepresentation, instruction.bitsize));
    return {0x64, };
}

std::vector<byte> Arc::PycEmitter::parseUnaryOperation(const Arc::IRUnaryOperationKind& op) {
    switch (op) {
        case Arc::IRUnaryOperationKind::Not: return {0x0F};
        case Arc::IRUnaryOperationKind::Negative: return {0x0B};
        case Arc::IRUnaryOperationKind::Positive: return {0x0A};
        case Arc::IRUnaryOperationKind::Ceil: return this->parseNamedCall("ceil", 1);
        case Arc::IRUnaryOperationKind::Floor: return this->parseNamedCall("floor", 1);
        case Arc::IRUnaryOperationKind::CountLeadingZeros: return {};
        case Arc::IRUnaryOperationKind::CountEndingZeros: return {};
        case Arc::IRUnaryOperationKind::CountLeadingOnes: return {};
        case Arc::IRUnaryOperationKind::CountEndingOnes: return {};
        case Arc::IRUnaryOperationKind::CountZeros: return {};
        case Arc::IRUnaryOperationKind::CountOnes: return {};
    }
}

std::vector<byte> Arc::PycEmitter::parseBinaryOperation(const Arc::IRBinaryOperationKind& op) {
    switch (op) {
        case Arc::IRBinaryOperationKind::Equal: return {0x6B, 2};
        case Arc::IRBinaryOperationKind::NotEqual: return {0x6B, 3};
        case Arc::IRBinaryOperationKind::Modulus: return {0x7A, 7};
        case Arc::IRBinaryOperationKind::Multiply: return {0x7A, 5};
        case Arc::IRBinaryOperationKind::Add: return {0x7A, 0};
        case Arc::IRBinaryOperationKind::Subtract: return {0x7A, 10};
        case Arc::IRBinaryOperationKind::Divide: return {0x7A, 8};
        case Arc::IRBinaryOperationKind::LessThan: return {0x6B, 0};
        case Arc::IRBinaryOperationKind::LessOrEqual: return {0x6B, 1};
        case Arc::IRBinaryOperationKind::GreaterThan: return {0x6B, 4};
        case Arc::IRBinaryOperationKind::GreaterOrEqual: return {0x6B, 5};
        case Arc::IRBinaryOperationKind::LeftShift: return {0x7A, 2};
        case Arc::IRBinaryOperationKind::RightShift: return {0x7A, 3};
        case Arc::IRBinaryOperationKind::BitwiseAnd: return {0x7A, 11};
        case Arc::IRBinaryOperationKind::LogicalAnd: return {0x7A, 11};
        case Arc::IRBinaryOperationKind::BitwiseOr: return {0x7A, 13};
        case Arc::IRBinaryOperationKind::LogicalOr: return {0x7A, 13};
        case Arc::IRBinaryOperationKind::BitwiseXOR: return {0x7A, 12};
        case Arc::IRBinaryOperationKind::InstanceOf: return {0x6B, 8};
        case Arc::IRBinaryOperationKind::LeftRotate: return {};
        case Arc::IRBinaryOperationKind::RightRotate: return {};
        case Arc::IRBinaryOperationKind::SquareRoot: return this->parseNamedCall("sqrt", 2, "math");
        case Arc::IRBinaryOperationKind::Power: return {0x7A, 4};
        case Arc::IRBinaryOperationKind::Maximum: return this->parseNamedCall("max", 2);
        case Arc::IRBinaryOperationKind::Minimum: return this->parseNamedCall("min", 2);
    }
}