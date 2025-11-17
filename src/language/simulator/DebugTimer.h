#pragma once
#include "../util/imports.h"

namespace Arc
{
    class DebugTimer
    {
    public:
        bool stopped = true;
        bool toBeToggled = false;
    private:
        std::chrono::time_point<std::chrono::steady_clock> startTimestamp;
        std::chrono::microseconds elapsedTime;
        bool timeUpdated = false;
    public:
        inline void start() {
            this->startTimestamp = std::chrono::steady_clock::now();
            this->timeUpdated = false;
            this->stopped = false;
        }

        inline void stop() {
            this->elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - this->startTimestamp);
            this->timeUpdated = true;
            this->stopped = true;
        }

        inline void reset() {
            this->elapsedTime = std::chrono::microseconds::zero();
            this->timeUpdated = true;
            this->stopped = true;
        }

        inline std::chrono::microseconds get() {
            if (!this->timeUpdated) {
                this->elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - this->startTimestamp);
            }

            return this->elapsedTime;
        }
    };
}