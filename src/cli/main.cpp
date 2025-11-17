#include "../language/Arc.h"
#include "Cli.h"
#include "imports.h"

int main(int argc, char* argv[]) {
    std::vector<std::string> arguments;

    for (size_t argIdx = 0; argIdx < argc; argIdx++) {
        arguments.push_back(std::string(argv[argIdx]));
    }

    ArcCLI::cliArguments = ArcCLI::CLIArguments {
        .count = argc,
        .arguments = arguments,
        .initialized = true
    };

    const ArcCLI::CLIArgumentParser cliArgumentParser;
    cliArgumentParser.parse();
    
    return 0;
}