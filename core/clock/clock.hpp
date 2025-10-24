#pragma once

#include <chrono>

using namespace std::chrono;

// Forward declaration
namespace Core { class System; }

namespace Core
{
    class Clock
    {
    protected:
        System* system;

        const int tick_interval;
        int tick_counter;
        int total_time;
        int quantum_interval;
        int quantum;
        bool ticked;
        bool running;

        time_point<system_clock> start_time;
        time_point<system_clock> end_time;
        duration<double> elapsed;

        virtual bool getTick() = 0;

    public:
        Clock(System* sys, int total_t = 0);
        virtual ~Clock();

        int getTotalTime() const { return total_time; }
        void setQuantum(const int q) { quantum_interval = q; }
        void resetQuantum() { quantum = quantum_interval; }

        void run();
        void stop();
    };
}