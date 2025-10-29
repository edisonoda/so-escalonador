#pragma once

#include "../../utils/std_libraries.hpp"

#include "clock_mode.hpp"

using namespace chrono;

namespace Core
{
    class AutoClock : public ClockMode
    {
    private:
        const int tick_interval;
        int tick_counter;
        bool ticked;

        time_point<system_clock> start_time;
        time_point<system_clock> end_time;
        duration<double> elapsed;

    public:
        AutoClock(Clock* clock, System* sys);
        ~AutoClock();

        virtual bool getTick();
    };
}