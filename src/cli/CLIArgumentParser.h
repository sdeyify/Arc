#pragma once
#include "imports.h"
#include "CLICommand.h"
#include "ProgramProcessingResult.h"
#include "ProgramProcessingCompletionStage.h"
#include "../language/Arc.h"

namespace ArcCLI
{
    class CLIArgumentParser
    {
    public:
        void parse() const;
    private:
        void printCommands() const;

        ProgramProcessingResult processProgram(const std::string& src, const std::string& filePath, const ProgramProcessingCompletionStage& completionStage) const;
        std::optional<ProgramProcessingResult> processFile(const ProgramProcessingCompletionStage& completionStage) const;
        void useProcessedIR(const std::function<void(const Arc::IRProgram&, const ProgramProcessingResult&)> fn, const std::string& errorMessage) const;

        std::optional<CLICommand> getCommandFromName(const std::string& name) const;

        std::optional<std::string> readFile(const std::string& path) const;

        void printProgram(const ArcCLI::ProgramProcessingCompletionStage& completionStage, const std::string& errorMessage) const;
        void printTokens(const Arc::TokenStream& tokenStream) const;
        void printAst(const Arc::AbstractSyntaxTree& ast) const;
        void printIR(const Arc::IRProgram& ir) const;
    };
}