#pragma once
#include "ir/IRNodes.h"
#include "../util/imports.h"

namespace Arc
{
    class IRProgram
    {
    public:
        IRProgram(
            const std::vector<Directive>& directives, const std::vector<Instruction>& instructions
        ) : constpool(loadDirectives(directives)), directives(directives), instructions(instructions) {}
    private:
        std::map<size_t, Directive> constpool;
        std::vector<Directive> directives;
        std::vector<Instruction> instructions;
    public:
        std::vector<Instruction> getInstructions() const;
        std::vector<Directive> getDirectives() const;

        std::optional<Directive> searchDirective(const size_t directiveId) const;
        std::optional<IRNodes::StructDirective> searchStructDirective(const size_t directiveId) const;
        std::optional<IRNodes::FunctionDirective> searchFunctionDirective(const size_t directiveId) const;
        std::optional<IRNodes::PointerDirective> searchPointerDirective(const size_t directiveId) const;
    private:
        std::map<size_t, Directive> loadDirectives(const std::vector<Directive>& constpool) const;
    };
}