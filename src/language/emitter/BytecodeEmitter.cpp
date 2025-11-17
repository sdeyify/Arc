#include "Emitter.h"
#include "../util/imports.h"

std::vector<byte> Arc::BytecodeEmitter::buildBytecode() {
    for (const Arc::Directive& directive : this->ir.getDirectives()) {
        this->parseDirective(directive);
    }
}

std::vector<byte> Arc::BytecodeEmitter::parseInstructions(const std::vector<Arc::Instruction>& instructions) {
    std::vector<byte> bytecode;

    for (const Arc::Instruction& instruction : instructions) {
        const std::vector<byte> instructionBytecode = this->parseInstruction(instruction);
        bytecode.insert(bytecode.end(), instructionBytecode.begin(), instructionBytecode.end());
    }

    return bytecode;
}

std::vector<Arc::Instruction> Arc::BytecodeEmitter::getInstructions(const size_t labelId) {
    std::vector<Arc::Instruction> instructions;
    bool seenLabel = false;

    for (const Arc::Instruction& instruction : this->ir.getInstructions()) {
        if (instruction->kind() == Arc::IRInstructionKind::Label) {
            seenLabel = true;
        }
        
        if (seenLabel) {
            instructions.push_back(instruction);
        }
    }

    return instructions;
}

void Arc::BytecodeEmitter::writeBytecode(const std::string& filepath) {
    std::ofstream file(filepath, std::ios::binary);

    if (!file) {
        // throw err
        std::unreachable();
        return;
    }

    const std::vector<byte> bytes = this->emitBytecode();
    const byte* byteBuffer = bytes.data();

    file.write(reinterpret_cast<const char*>(byteBuffer), bytes.size());
}

void Arc::BytecodeEmitter::parseDirective(const Arc::Directive& directive) {
    switch (directive->kind()) {
        case Arc::IRDirectiveKind::FunctionDirective: this->parseFunction(*directive->cast<Arc::IRNodes::FunctionDirective>()); break;
        case Arc::IRDirectiveKind::StructDirective: this->parseStruct(*directive->cast<Arc::IRNodes::StructDirective>()); break;
        case Arc::IRDirectiveKind::PointerDirective: this->parsePointerDirective(*directive->cast<Arc::IRNodes::PointerDirective>()); break;
    }
}