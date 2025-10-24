#pragma once

#include <chrono>

using namespace std::chrono;

// Forward declaration
namespace Core { class System; }

namespace Core
{
    class SystemClock
    {
    private:
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

        bool getTick();

    public:
        SystemClock(System* sys);
        ~SystemClock();

        int getTotalTime() const { return total_time; }
        void setQuantum(const int q) { quantum_interval = q; }
        void resetQuantum() { quantum = quantum_interval; }

        void run();
        void stop();
    };
}