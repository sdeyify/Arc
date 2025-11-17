#pragma once
#include "../parser/Parser.h"
#include "../util/Util.h"

namespace Arc
{
    class BytecodeEmitter
    {
    public:
        BytecodeEmitter(const Arc::IRProgram& ir) : ir(ir) {};
    protected:
        const Arc::IRProgram& ir;
    public:
        void writeBytecode(const std::string& filepath);
        virtual std::vector<byte> emitBytecode() = 0;
    protected:
        std::vector<byte> buildBytecode();

        std::vector<Instruction> getInstructions(const size_t labelId);
        std::vector<byte> parseInstructions(const std::vector<Instruction>& instructions);
        virtual std::vector<byte> parseInstruction(const Instruction& instruction) = 0;

        virtual void parseFunction(const Arc::IRNodes::FunctionDirective& directive) = 0;
        virtual void parseStruct(const Arc::IRNodes::StructDirective& directive) = 0;
        virtual void parsePointerDirective(const Arc::IRNodes::PointerDirective& directive) = 0;

        void parseDirective(const Arc::Directive& directive);

        virtual byte parseBitsize(const Arc::IRBitsizeKind& bitsize) = 0;

        virtual std::vector<byte> getStringifiedBitvalue(const std::string& raw, const Arc::IRBitsizeKind& bitsize) = 0;
    };
}