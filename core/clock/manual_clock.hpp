#pragma once

#include "clock.hpp"

namespace Core
{
    class ManualClock : public Clock
    {
    private:
        virtual bool getTick();

    public:
        ManualClock(System* sys, int total_t = 0);
        ~ManualClock();
    };
}