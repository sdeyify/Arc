#include "Parser.h"
#include "../util/imports.h"

std::vector<Arc::Instruction> Arc::IRProgram::getInstructions() const {
    return this->instructions;
}

std::vector<Arc::Directive> Arc::IRProgram::getDirectives() const {
    return this->directives;
}

std::map<size_t, Arc::Directive> Arc::IRProgram::loadDirectives(const std::vector<Arc::Directive>& constpool) const {
    std::map<size_t, Arc::Directive> mappedConstpool;

    for (const Arc::Directive& directive : constpool) {
        mappedConstpool[directive->id] = directive;
    }

    return mappedConstpool;
}

std::optional<Arc::Directive> Arc::IRProgram::searchDirective(const size_t directiveId) const {
    return (this->constpool.contains(directiveId))
        ? std::make_optional(this->constpool.at(directiveId))
        : std::nullopt;
}

std::optional<Arc::IRNodes::FunctionDirective> Arc::IRProgram::searchFunctionDirective(const size_t directiveId) const {
    const std::optional<Arc::Directive> foundDirective = this->searchDirective(directiveId);

    return (foundDirective && (*foundDirective)->kind() == Arc::IRDirectiveKind::FunctionDirective)
        ? std::make_optional(*(*foundDirective)->cast<Arc::IRNodes::FunctionDirective>())
        : std::nullopt;
}

std::optional<Arc::IRNodes::StructDirective> Arc::IRProgram::searchStructDirective(const size_t directiveId) const {
    const std::optional<Arc::Directive> foundDirective = this->searchDirective(directiveId);

    return (foundDirective && (*foundDirective)->kind() == Arc::IRDirectiveKind::StructDirective)
        ? std::make_optional(*(*foundDirective)->cast<Arc::IRNodes::StructDirective>())
        : std::nullopt;
}

std::optional<Arc::IRNodes::PointerDirective> Arc::IRProgram::searchPointerDirective(const size_t directiveId) const {
    const std::optional<Arc::Directive> foundDirective = this->searchDirective(directiveId);

    return (foundDirective && (*foundDirective)->kind() == Arc::IRDirectiveKind::PointerDirective)
        ? std::make_optional(*(*foundDirective)->cast<Arc::IRNodes::PointerDirective>())
        : std::nullopt;
}