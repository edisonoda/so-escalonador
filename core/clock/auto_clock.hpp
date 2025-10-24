#pragma once

#include "clock.hpp"

namespace Core
{
    class AutoClock : public Clock
    {
    private:
        virtual bool getTick();

    public:
        AutoClock(System* sys, int total_t = 0);
        ~AutoClock();
    };
}