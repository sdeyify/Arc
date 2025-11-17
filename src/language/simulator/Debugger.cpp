#pragma once
#include "Simulator.h"
#include "../util/imports.h"

void Arc::Debugger::welcomeMessage() const {
    if (!this->simulator.displayedBreakpointMessage) {
        std::cout << "---- Arc Debugger ----" << "\n";
        std::cout << "Enter 'help' to get help information." << "\n";
    }

    this->simulator.displayedBreakpointMessage = true;
}

void Arc::Debugger::debug(const size_t instructionPtr) {
    while (true) {
        this->getInput();
        const bool exit = this->parseInput(instructionPtr);

        if (exit) break;
    }
}

bool Arc::Debugger::parseInput(const size_t instructionPtr) {
    this->stopAllTimers();
    this->updateLabels(instructionPtr);

    if (Arc::debugInput.empty()) {
        std::cout << "No command has been supplied." << std::endl;
    } else {
        const std::string command = Arc::debugInput.next();

        if (command == "help") {
            if (Arc::debugInput.empty()) {
                this->printCommands();
            } else {
                const std::string commandName = Arc::debugInput.next();

                const std::optional<Arc::DebugCommand> possibleCommand = this->getCommandFromName(commandName);

                if (possibleCommand) {
                    const Arc::DebugCommand dbgCommand = *possibleCommand;

                    std::cout << dbgCommand.name << ": " << dbgCommand.description << " usage: " << dbgCommand.usage << std::endl;
                } else {
                    std::cout << "Command " + commandName + " not found. Please check if you have made a typo or if you have the right version of Arc.";
                    this->printCommands();
                }
            }
        } else if (command == "eval") {
            this->simulator.parseReplBreakpoint();
        } else if (command == "watch") {
            this->handleWatchCommand();
        } else if (command == "label") {
            this->handleLabelCommand();
        } else if (command == "timer") {
            this->handleTimerCommand();
        } else if (command == "continue") {
            return true;
        } else {
            std::cout << "Command " + command + " not found. Please check if you have made a typo or if you have the right version of Arc.";
            this->printCommands();
        }
    }

    this->updateTimers();

    return false;
}

void Arc::Debugger::updateTimers() {
    for (const std::string& timerName : std::views::keys(this->timers)) {
        Arc::DebugTimer timer = this->timers[timerName];

        if (timer.toBeToggled) {
            const int64_t elapsedTime = timer.get().count();
            std::cout << timerName << ": " << "milliseconds: " << elapsedTime << " seconds: " << elapsedTime / 1'000'000 << "\n";
            if (timer.stopped) timer.start(); else timer.stop();
        }
    }
}

void Arc::Debugger::stopAllTimers() {
    for (Arc::DebugTimer& timer : std::views::values(this->timers)) {
        timer.stop();
    }
}

void Arc::Debugger::updateLabels(const size_t instructionPtr) {
    for (Arc::DebugLabel& label : std::views::values(this->labels)) {
        if (label.timerLink && label.instructionPtr == instructionPtr) {
            const std::string timerName = label.timerLink.value().timerName;
            this->timers[timerName].start();
        }
    }
}

void Arc::Debugger::handleLabelCommand() {
    const std::string labelCommand = Arc::debugInput.expect("");

    if (labelCommand == "new") {
        this->handleLabelCreate();
    } else if (labelCommand == "remove") {
        this->handleLabelRemove();
    } else if (labelCommand == "jump") {
        this->handleLabelJump();
    } else if (labelCommand == "all") {
        this->handleLabelAll();
    } else if (labelCommand == "marklog") {
        this->handleLabelMarklog();
    } else {
        // throw err
        std::unreachable();
    }
}

void Arc::Debugger::handleTimerCommand() {
    const std::string labelCommand = Arc::debugInput.expect("");

    if (labelCommand == "start") {
        this->handleTimerStart();
    } else if (labelCommand == "end") {
        this->handleTimerStop();
    } else if (labelCommand == "startpoint") {
        this->handleTimerStartpoint();
    } else if (labelCommand == "endpoint") {
        this->handleTimerEndpoint();
    } else if (labelCommand == "get") {
        this->handleTimerGet();
    } else if (labelCommand == "reset") {
        this->handleTimerReset();
    } else {
        // throw err
        std::unreachable();
    }
}

void Arc::Debugger::handleTimerStartpoint() {
    const std::string timerName = Arc::debugInput.expect("");
    const std::string labelName = Arc::debugInput.expect("");

    this->labels[labelName].timerLink = std::make_optional(Arc::DebugTimerLink(Arc::DebugTimerPoint::Start, timerName));
}

void Arc::Debugger::handleTimerEndpoint() {
    const std::string timerName = Arc::debugInput.expect("");
    const std::string labelName = Arc::debugInput.expect("");

    this->labels[labelName].timerLink = std::make_optional(Arc::DebugTimerLink(Arc::DebugTimerPoint::End, timerName));
}

void Arc::Debugger::handleTimerStart() {
    const std::string timerName = Arc::debugInput.expect("");
    if (this->timers[timerName].stopped) this->timers[timerName].toBeToggled = true;
}

void Arc::Debugger::handleTimerStop() {
    const std::string timerName = Arc::debugInput.expect("");
    if (!this->timers[timerName].stopped) this->timers[timerName].toBeToggled = true;
}

void Arc::Debugger::handleTimerGet() {
    const std::string timerName = Arc::debugInput.expect("");
    const int64_t elapsedTime = this->timers[timerName].get().count();
    std::cout << timerName << ": " << "milliseconds: " << elapsedTime << " seconds: " << elapsedTime / 1'000'000 << "\n";
}

void Arc::Debugger::handleTimerReset() {
    const std::string timerName = Arc::debugInput.expect("");
    this->timers[timerName].reset();
}

void Arc::Debugger::handleLabelAll() {
    for (const std::string& debugLabel : std::views::keys(this->labels)) {
        std::cout << debugLabel << " at " << this->labels[debugLabel].instructionPtr;

        if (this->labels[debugLabel].logMessage) {
            std::cout << " logs " << *this->labels[debugLabel].logMessage;
        }

        if (this->labels[debugLabel].timerLink) {
            std::cout << ((this->labels[debugLabel].timerLink.value().point == Arc::DebugTimerPoint::Start) ? " startpoint" : " endpoint");
            std::cout << " of timer " << this->labels[debugLabel].timerLink.value().timerName;
        }
    }
}

void Arc::Debugger::handleLabelMarklog() {
    const std::string labelName = Arc::debugInput.expect("");
    const std::string logMessage = Arc::debugInput.expect("");

    this->labels[labelName].logMessage = std::make_optional(logMessage);
}

void Arc::Debugger::handleLabelRemove() {
    const std::string labelName = Arc::debugInput.expect("");

    this->labels.erase(labelName);
}

void Arc::Debugger::handleLabelJump() {
    const std::string labelName = Arc::debugInput.expect("");

    this->simulator.updateStackLocation(this->labels[labelName].instructionPtr);
}

void Arc::Debugger::handleLabelCreate() {
    const std::string labelName = Arc::debugInput.expect("");

    this->labels.insert(
        std::make_pair(labelName,
            Arc::DebugLabel(
                this->simulator.getTopStackLocation()
            )
        )
    );
}

void Arc::Debugger::handleWatchCommand() {
    const std::string watchCommand = Arc::debugInput.expect("");

    if (watchCommand == "new") {
        this->handleWatchCreate();
    } else if (watchCommand == "get") {
        this->handleWatchGet();
    } else if (watchCommand == "remove") {
        this->handleWatchRemove();
    } else if (watchCommand == "getall") {
        this->handleWatchAll();
    } else {
        // throw err
        std::unreachable();
    }
}

void Arc::Debugger::handleWatchAll() {
    for (const std::string& watchName : std::views::keys(this->watches)) {
        std::cout << "watch " << watchName << ": " << this->watches[watchName] << std::endl;
    }
}

void Arc::Debugger::handleWatchRemove() {
    const std::string watchName = Arc::debugInput.expect("");

    if (!this->watches.contains(watchName)) {
        // throw err
        std::unreachable();
    }

    this->watches.erase(watchName);
}

void Arc::Debugger::handleWatchCreate() {
    const std::string watchSrc = Arc::debugInput.expect("");

    if (Arc::debugInput.expect("") != "as") {
        // throw err
        std::unreachable();
    }

    const std::string watchName = Arc::debugInput.expect("");

    this->watches[watchName] = watchSrc;
}

void Arc::Debugger::handleWatchGet() {
    const std::string watchName = Arc::debugInput.expect("");

    if (!this->watches.contains(watchName)) {
        // throw err
        std::unreachable();
    }

    this->simulator.evaluateCode(this->watches[watchName]);
    std::cout << watchName << ": " << this->simulator.inspectValue(this->simulator.stack.back());
}

void Arc::Debugger::getInput() const {
    std::string input;
    std::cin >> input;
    Arc::debugInput = Arc::DebugInput::fromString(input);
}

std::optional<Arc::DebugCommand> Arc::Debugger::getCommandFromName(const std::string& name) const {
    for (const Arc::DebugCommand& command : Arc::debugCommands) {
        if (command.name == name) return std::make_optional(command);
    }

    return std::nullopt;
}

void Arc::Debugger::printCommands() const {
    for (const Arc::DebugCommand& dbgCommand : Arc::debugCommands) {
        std::cout << dbgCommand.name << ": " << dbgCommand.description << " usage: " << dbgCommand.usage << std::endl;
    }
}