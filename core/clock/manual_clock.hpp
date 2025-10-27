#pragma once

#include "clock_mode.hpp"

namespace Core
{
    class ManualClock : public ClockMode
    {
    public:
        ManualClock(Clock* clock, System* sys);
        ~ManualClock();
        
        virtual bool getTick();
    };
}