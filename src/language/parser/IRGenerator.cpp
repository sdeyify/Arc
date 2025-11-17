#include "Parser.h"
#include "../util/Util.h"

Arc::IRProgram Arc::IRGenerator::generateIR(const Arc::AbstractSyntaxTree& ast) {
    const Arc::IRProgram generatedIR(std::vector<Arc::Directive>{}, std::vector<Arc::Instruction>{});
    return generatedIR;
}