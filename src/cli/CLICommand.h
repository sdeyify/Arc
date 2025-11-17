#pragma once
#include "imports.h"

// aggregate initialization causes an ICE (internal compiler error)
// probably because of const vector + aggregate initialization
// to be fixed in Visual Studio 17.8

namespace ArcCLI
{
    struct CLICommand
    {
        // aggregate initialization causes an ICE
        CLICommand(
            const std::string& name, const std::string& description, const std::string& usage
        ) : name(name), description(description), usage(usage) {}

        const std::string& name;
        const std::string& description;
        const std::string& usage;
    };
    
    const inline std::vector<CLICommand> cliCommands = {
        CLICommand("tokenize", "Prints the lexed/tokenized form of the source code.", "arc tokenize <File Path>"),
        CLICommand("ast", "Prints the AST (Abstract Syntax Tree) form of the source code.", "arc ast <File Path>"),
        CLICommand("ir", "Prints the Arc IR (Arc Intermediate Representation) form of the source code.", "arc ir <File Path>"),
        CLICommand("simulate", "Simulates the source code.", "arc simulate <Path>"),
        CLICommand("debug", "Simulates the source code in debug mode.", "arc debug <Path>"),
        CLICommand("compile", "Compiles the source code.", "arc compile <Path> ('native': default | 'windows' | 'elf' | 'darwin' | 'jvm' | 'msil' | 'pyc' | 'android' | 'wasm')"),
        CLICommand("repl", "Opens an Arc REPL (Read Evaluate Print Loop)", "arc repl"),
        CLICommand("help", "Prints the avaliable commands if no command is given or the description of a command if one is given.", "arc help (optional: <Command>)"),
        CLICommand("testping", "Internal command to check if the cli is running. Always outputs 'OK'.", "arc testping")
    };
}