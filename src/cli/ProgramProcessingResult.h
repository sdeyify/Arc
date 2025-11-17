#pragma once
#include "ProgramProcessingCompletionStage.h"
#include "../language/Arc.h"
#include "imports.h"

namespace ArcCLI
{
    struct ProgramProcessingResult
    {
        const std::string& src;
        const std::string& filePath;
        const ProgramProcessingCompletionStage& completionStage;
        std::optional<Arc::TokenStream> lexingResult;
        std::optional<Arc::AbstractSyntaxTree> astBuildingResult;
        std::optional<Arc::IRProgram> irGenerationResult;
    };
}