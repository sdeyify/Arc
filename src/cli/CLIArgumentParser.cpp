#include "Cli.h"
#include "../language/Arc.h"
#include <iostream>
#include <fstream>

void ArcCLI::CLIArgumentParser::parse() const {
    ArcCLI::cliArguments.next();

    if (ArcCLI::cliArguments.empty()) {
        this->printCommands();
    } else {
        const std::string command = ArcCLI::cliArguments.next();

        if (command == "tokenize") {
            this->printProgram(ArcCLI::ProgramProcessingCompletionStage::Lexer, "An internal error happened while processing your program.");
        } else if (command == "ast") {
            this->printProgram(ArcCLI::ProgramProcessingCompletionStage::AbstractSyntaxTree, "" /* TODO: Add a good error message for this */);
        } else if (command == "ir") {
            this->printProgram(ArcCLI::ProgramProcessingCompletionStage::IntermediateRepresentation, "" /* TODO: Add a good error message for this */);
        } else if (command == "simulate") {
            this->useProcessedIR([](const Arc::IRProgram& ir, const ArcCLI::ProgramProcessingResult& result) {
                Arc::ArcSimulator simulator(ir, Arc::SimulationEnvironment {
                    .structNames = {},
                    .functionNames = {},
                    .pointerDirectiveValueKinds = {},
                    .environmentVariables = {},
                    .breakpointEnvironment = Arc::BreakpointEnvironment::Ordinary,
                    .path = result.filePath
                });

                simulator.simulate();
            }, "" /* TODO: Add a good error message for this */);
        } else if (command == "debug") {
            this->useProcessedIR([](const Arc::IRProgram& ir, const ArcCLI::ProgramProcessingResult& result) {
                Arc::ArcSimulator simulator(ir, Arc::SimulationEnvironment {
                    .structNames = {},
                    .functionNames = {},
                    .pointerDirectiveValueKinds = {},
                    .environmentVariables = {},
                    .breakpointEnvironment = Arc::BreakpointEnvironment::Debug,
                    .path = result.filePath,
                });

                simulator.simulate();
            }, "" /* TODO: Add a good error message for this */);
        } else if (command == "compile") {
            this->useProcessedIR([this](const Arc::IRProgram& ir, const ArcCLI::ProgramProcessingResult result) {
                const std::map<std::string, ArcCLI::ProgramProcessingCompletionStage> completionStageMap = {
                    {"darwin", ArcCLI::ProgramProcessingCompletionStage::CompileDarwin},
                    {"android", ArcCLI::ProgramProcessingCompletionStage::CompileDEX},
                    {"elf", ArcCLI::ProgramProcessingCompletionStage::CompileELF},
                    {"jvm", ArcCLI::ProgramProcessingCompletionStage::CompileJVM},
                    {"msil", ArcCLI::ProgramProcessingCompletionStage::CompileMSIL},
                    {"windows", ArcCLI::ProgramProcessingCompletionStage::CompilePE},
                    {"pyc", ArcCLI::ProgramProcessingCompletionStage::CompilePYC},
                    {"wasm", ArcCLI::ProgramProcessingCompletionStage::CompileWASM}
                };

                const std::string target = ArcCLI::cliArguments.next();

                if (!completionStageMap.contains(target)) std::cout << "" /* TODO: Add a good error message for this */ << "\n"; return;

                const ArcCLI::ProgramProcessingCompletionStage completionStage = completionStageMap.at(target);

                this->processProgram(result.src, result.filePath, completionStage);
            }, "" /* TODO: Add a good error message for this */);
        } else if (command == "repl") {
            
        } else if (command == "testping") {
            std::cout << "OK" << std::endl;
        } else if (command == "help") {
            ArcCLI::cliArguments.next();

            if (!ArcCLI::cliArguments.empty()) {
                const std::string commandName = ArcCLI::cliArguments.next();
                const std::optional<ArcCLI::CLICommand> command = this->getCommandFromName(commandName);

                if (command) {
                    std::cout << command.value().name << ": " << command.value().description << " usage: " << command.value().usage << std::endl;
                } else {
                    std::cout << "Command " + commandName + " not found. Please check if you have made a typo or if you have the right version of Arc.\n";
                    this->printCommands();
                }
            } else {
                this->printCommands();
            }
        } else {
            std::cout << "Command " + command + " not found. Please check if you have made a typo or if you have the right version of Arc.";
            this->printCommands();
        }
    }

    std::cout << std::flush; 
}

void ArcCLI::CLIArgumentParser::useProcessedIR(const std::function<void(const Arc::IRProgram& ir, const ArcCLI::ProgramProcessingResult& src)> fn, const std::string& errorMessage) const {
    const std::optional<ArcCLI::ProgramProcessingResult> possibleResult = this->processFile(ArcCLI::ProgramProcessingCompletionStage::IntermediateRepresentation);

    if (possibleResult) {
        const std::optional<Arc::IRProgram> possibleIR = possibleResult.value().irGenerationResult;

        if (possibleIR) {
            fn(*possibleIR, *possibleResult);
        }
    }
    
    std::cout << errorMessage << std::endl;
}

std::optional<ArcCLI::ProgramProcessingResult> ArcCLI::CLIArgumentParser::processFile(const ArcCLI::ProgramProcessingCompletionStage& completionStage) const {
    const std::string filePath = ArcCLI::cliArguments.next();
    const std::optional<std::string> possibleFileContents = this->readFile(filePath);

    if (possibleFileContents) {
        return std::make_optional(this->processProgram(*possibleFileContents, filePath, completionStage));
    } else {
        std::cout << "Could not find file " + filePath + ".";
    }

    return std::nullopt;
}

void ArcCLI::CLIArgumentParser::printProgram(const ArcCLI::ProgramProcessingCompletionStage& completionStage, const std::string& errorMessage) const {
    const std::optional<ArcCLI::ProgramProcessingResult> possibleResult = this->processFile(completionStage);

    if (possibleResult) {
        const ArcCLI::ProgramProcessingResult result = *possibleResult;

        switch (completionStage) {
            case ArcCLI::ProgramProcessingCompletionStage::Lexer: if (!result.lexingResult) std::cout << errorMessage << std::endl; else this->printTokens(*result.lexingResult); break;
            case ArcCLI::ProgramProcessingCompletionStage::AbstractSyntaxTree: if (!result.astBuildingResult) std::cout << errorMessage << std::endl; else this->printAst(*result.astBuildingResult); break;
            case ArcCLI::ProgramProcessingCompletionStage::IntermediateRepresentation: if (!result.irGenerationResult) std::cout << errorMessage << std::endl; else this->printIR(*result.irGenerationResult); break;
            default: break;
        }
    } else {
        // throw err
    }
}

ArcCLI::ProgramProcessingResult ArcCLI::CLIArgumentParser::processProgram(const std::string& src, const std::string& filePath, const ArcCLI::ProgramProcessingCompletionStage& completionStage) const {
    ArcCLI::ProgramProcessingResult result = ArcCLI::ProgramProcessingResult {
        .src = src,
        .filePath = filePath,
        .completionStage = completionStage,
        .lexingResult = std::nullopt,
        .astBuildingResult = std::nullopt,
        .irGenerationResult = std::nullopt
    };

    const Arc::Lexer lexer(src);
    Arc::TokenStream tokenStream = lexer.parse();
    result.lexingResult = std::make_optional(tokenStream);
    if (completionStage == ArcCLI::ProgramProcessingCompletionStage::Lexer) return result;

    Arc::ASTBuilder astBuilder;
    const Arc::AbstractSyntaxTree ast = astBuilder.parse(tokenStream, filePath);
    result.astBuildingResult = std::make_optional(ast);
    if (completionStage == ArcCLI::ProgramProcessingCompletionStage::AbstractSyntaxTree) return result;

    Arc::IRGenerator irGenerator;
    const Arc::IRProgram ir = irGenerator.generateIR(ast);
    result.irGenerationResult = std::make_optional(ir);
    if (completionStage == ArcCLI::ProgramProcessingCompletionStage::IntermediateRepresentation) return result;

    return result;
}

std::optional<ArcCLI::CLICommand> ArcCLI::CLIArgumentParser::getCommandFromName(const std::string& name) const {
    for (const ArcCLI::CLICommand& validCommand : ArcCLI::cliCommands) {
        if (validCommand.name == name) return std::make_optional(validCommand);
    }

    return std::nullopt;
}

void ArcCLI::CLIArgumentParser::printCommands() const {
    std::cout << std::endl;
    std::cout << "---- Arc CLI Commands ----" << "\n" << std::endl;

    std::cout << "Name | Description | Usage" << "\n";

    std::cout << "\n";

    for (const ArcCLI::CLICommand& validCommand : ArcCLI::cliCommands) {
        std::cout << validCommand.name << " | " << validCommand.description << " | " << validCommand.usage << "\n";
    }

    std::cout << std::endl;
}

std::optional<std::string> ArcCLI::CLIArgumentParser::readFile(const std::string& path) const {
    return std::make_optional(std::string("1 + 1"));

    std::ifstream fileStream(path);

    if (fileStream.fail()) {
        return std::nullopt;
    }

    std::string fileContents;
    std::string fileLine;
    
    while (std::getline(fileStream, fileLine)) {
        fileContents += fileLine;
        fileContents += "\n";
    }

    return std::make_optional(fileContents);
}

void ArcCLI::CLIArgumentParser::printTokens(const Arc::TokenStream& tokenStream) const {
    for (const Arc::Token& token : tokenStream.getTokens()) {
        std::cout << "[" << Arc::stringifyTokenType(token.kind) << "] " << token.value << " at line: " << token.line << " and column: " << token.col << "\n";
    }

    std::cout << std::endl;
}