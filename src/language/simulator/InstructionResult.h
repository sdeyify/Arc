#pragma once
#include "InstructionResultKind.h"
#include "../util/imports.h"

namespace Arc
{
    struct InstructionResult
    {
        const Arc::InstructionResultKind& kind;
        const std::optional<int>& possibleExitCode;

        static InstructionResult createOrdinary() {
            return InstructionResult {
                .kind = Arc::InstructionResultKind::OrdinaryResult,
                .possibleExitCode = std::nullopt
            };
        }

        static InstructionResult createExit(const int exitCode) {
            return InstructionResult {
                .kind = Arc::InstructionResultKind::ProgramExitResult,
                .possibleExitCode = std::make_optional(exitCode)
            };
        }

        static InstructionResult createReturn() {
            return InstructionResult {
                .kind = Arc::InstructionResultKind::ReturnResult,
                .possibleExitCode = std::nullopt
            };
        }
    };
}