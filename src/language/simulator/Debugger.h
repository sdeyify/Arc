#pragma once
#include "../util/imports.h"
#include "ArcSimulator.h"
#include "DebugCommand.h"
#include "DebugLabel.h"
#include "DebugTimer.h"

namespace Arc
{
    class Debugger
    {
    public:
        Debugger(ArcSimulator& simulator) : simulator(simulator) {}
    private:
        ArcSimulator& simulator;

        std::map<std::string, std::string> watches;
        std::map<std::string, DebugLabel> labels;
        std::map<std::string, DebugTimer> timers;
    public:
        void welcomeMessage() const;
        void debug(const size_t instructionPtr);
    private:
        bool parseInput(const size_t instructionPtr);
        void getInput() const;
        void printCommands() const;

        void handleWatchCommand();
        void handleLabelCommand();
        void handleTimerCommand();

        void handleTimerStart();
        void handleTimerStop();
        void handleTimerStartpoint();
        void handleTimerEndpoint();
        void handleTimerGet();
        void handleTimerReset();

        void handleLabelCreate();
        void handleLabelRemove();
        void handleLabelAll();
        void handleLabelJump();
        void handleLabelMarklog();

        void handleWatchCreate();
        void handleWatchRemove();
        void handleWatchAll();
        void handleWatchGet();

        void stopAllTimers();
        void updateLabels(const size_t instructionPtr);
        void updateTimers();

        std::optional<DebugCommand> getCommandFromName(const std::string& name) const;
    };
}