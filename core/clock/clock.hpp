#pragma once

#include "clock_mode.hpp"

#include "../../ui/screen.hpp"

// Forward declaration
namespace Core { class System; }

namespace Core
{
    class Clock
    {
    private:
        System* system;
        ClockMode* mode;
        UI::Screen* screen;
        
        int total_time;
        int quantum_interval;
        int quantum;
        bool running;

    public:
        Clock(System* sys);
        virtual ~Clock();

        int getQuantum() const { return quantum_interval; }
        void setQuantum(const int q) { quantum_interval = q; }
        void resetQuantum() { quantum = quantum_interval; }
        int getTotalTime() const { return total_time; }

        void selectMode(char mode);
        void run();
        void stop();
    };
}