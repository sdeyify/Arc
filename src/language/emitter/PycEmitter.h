#pragma once
#include "BytecodeEmitter.h"
#include "PythonCodeObject.h"
#include "../parser/Parser.h"
#include "../util/imports.h"

namespace Arc
{
    class PycEmitter : public BytecodeEmitter
    {
    public:
        PycEmitter(const Arc::IRProgram& ir) : BytecodeEmitter(ir) {};
    private:
        const byte flag = 0x00000000;

        PythonCodeObject currentCodeObject;
        std::stack<PythonCodeObject> codeObjects;
    private:
        std::vector<byte> parseInstruction(const Instruction& instruction) override;

        std::vector<byte> getMagicNumber(const byte version);
        std::vector<byte> getTimestamp(const std::filesystem::path& filepath);

        void parseFunction(const Arc::IRNodes::FunctionDirective& directive) override;
        void parseStruct(const Arc::IRNodes::StructDirective& directive) override;
        void parsePointerDirective(const Arc::IRNodes::PointerDirective& directive) override;

        byte parseBitsize(const Arc::IRBitsizeKind& bitsize) override;

        template<typename T>
        std::vector<byte> numToBytes(const T& value) {
            uint16_t value = 0x1234;
            uint8_t* p = reinterpret_cast<uint8_t*>(&value);

            uint8_t low  = p[0];
            uint8_t high = p[1];

            return {low, high};
        }

        std::vector<byte> getStringifiedBitvalue(const std::string& raw, const Arc::IRBitsizeKind& bitsize) override;

        std::vector<byte> parseLoadValue(const Arc::IRNodes::LoadValue& instruction);
        std::vector<byte> parseNamedCall(const std::string& fnName, const size_t argCount, const std::string& module = "");
        std::vector<byte> parseUnaryOperation(const Arc::IRUnaryOperationKind& op);
        std::vector<byte> parseBinaryOperation(const Arc::IRBinaryOperationKind& op);
    };
}