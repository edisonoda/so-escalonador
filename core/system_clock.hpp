#pragma once

#include <chrono>

using namespace std::chrono;

namespace Core
{
    class SystemClock
    {
    private:
        int tick_interval;
        int tick_counter;
        bool ticked;

        time_point<system_clock> start_time;
        time_point<system_clock> end_time;
        duration<double> elapsed;

    public:
        SystemClock();
        ~SystemClock();

        bool getTick();
    };
}