#pragma once

namespace ArcCLI
{
    enum class ProgramProcessingCompletionStage
    {
        Lexer,
        AbstractSyntaxTree,
        IntermediateRepresentation,
        CompilePE,
        CompileELF,
        CompileDarwin,
        CompileJVM,
        CompileMSIL,
        CompilePYC,
        CompileDEX,
        CompileWASM
    };
}