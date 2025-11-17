#pragma once
#include "BytecodeEmitter.h"
#include "WasmBitsize.h"
#include "../parser/Parser.h"
#include "../util/imports.h"

namespace Arc
{
    class WasmEmitter : public BytecodeEmitter
    {
    public:
        WasmEmitter(const Arc::IRProgram& ir) : BytecodeEmitter(ir) {};
    private:
        const byte magicHeader[4] = {0x00, 0x61, 0x73, 0x6D};
        const byte versionNumber[4] = {0x01, 0x00, 0x00, 0x00};

        const std::map<WasmBitsize, std::map<WasmBitsize, std::vector<byte>>> castTable = {
            {WasmBitsize::I32, {
                {WasmBitsize::I32, {}},
                {WasmBitsize::I64, {0xAC}},
                {WasmBitsize::F32, {0xB2}},
                {WasmBitsize::F64, {0xAC, 0xB9}}
            }},
            {WasmBitsize::I64, {
                {WasmBitsize::I32, {0xA7}},
                {WasmBitsize::I64, {0}},
                {WasmBitsize::F32, {0xB4}},
                {WasmBitsize::F64, {0xB9}}
            }},
            {WasmBitsize::F32, {
                {WasmBitsize::I32, {0xA8}},
                {WasmBitsize::I64, {0xAE}},
                {WasmBitsize::F32, {}},
                {WasmBitsize::F64, {0xB6}}
            }},
            {WasmBitsize::F64, {
                {WasmBitsize::I32, {0xAA}},
                {WasmBitsize::I64, {0xB0}},
                {WasmBitsize::F32, {0x9D}},
                {WasmBitsize::F64, {0xBB}}
            }},
        };

        std::vector<byte> typeSection;
        std::vector<byte> functionSection;
        std::vector<byte> globalSection;
        std::vector<byte> codeSection;

        std::map<size_t, byte> functionIds;
        byte highestFunctionId = 0x00;

        std::map<size_t, byte> structIds;
        byte highestStructId = 0x00;

        std::map<size_t, byte> globalIds;
        byte highestGlobalId = 0x00;

        byte highestPointerId = 0x00;
    private:
        std::vector<byte> emitBytecode() override;

        byte parseBitsize(const Arc::IRBitsizeKind& bitsize) override;
        byte parseLoadBitsize(const Arc::WasmBitsize& bitsize);
        WasmBitsize getWasmBitsize(const Arc::IRBitsizeKind& bitsize);

        template<typename T>
        T getOnWasmBitsize(const Arc::WasmBitsize& bitsize, const T f32Value, const T f64Value, const T i32Value, const T i64Value) {
            switch (bitsize) {
                case Arc::WasmBitsize::F32: return f32Value;
                case Arc::WasmBitsize::F64: return f64Value;
                case Arc::WasmBitsize::I32: return i32Value;
                case Arc::WasmBitsize::I64: return i64Value;
            }
        }

        std::vector<byte> getStringifiedBitvalue(const std::string& raw, const Arc::IRBitsizeKind& bitsize) override;

        template <typename T>
        std::vector<byte> numToBytes(const T& value);

        std::vector<byte> parseInstruction(const Instruction& instruction) override;

        void parseFunction(const Arc::IRNodes::FunctionDirective& directive) override;
        void parseStruct(const Arc::IRNodes::StructDirective& directive) override;
        void parsePointerDirective(const Arc::IRNodes::PointerDirective& directive) override;

        std::vector<byte> parseBinaryOperation(const Arc::IRNodes::BinaryOperation& instruction);
        std::vector<byte> parseUnaryOperation(const Arc::IRNodes::UnaryOperation& instruction);
        std::vector<byte> parseLoadPointer(const Arc::IRBitsizeKind& bitsize);
        std::vector<byte> parseStorePointer(const Arc::IRBitsizeKind& bitsize);
        std::vector<byte> parsePointer(const Arc::IRNodes::Pointer& instruction);
        std::vector<byte> parseBitcast(const Arc::IRNodes::Bitcast& instruction);
        std::vector<byte> parseNewObject(const Arc::IRNodes::NewObject& instruction);
        std::vector<byte> parseLoadValue(const Arc::IRNodes::LoadValue& instruction);
        std::vector<byte> parseBubble(const size_t stackAddress);
    };
}