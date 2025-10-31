#pragma once

#include "../../ui/window.hpp"

// Forward declaration
namespace Core { class Clock; }
namespace Core { class System; }

namespace Core
{
    class ClockMode
    {
    protected:
        Clock* clock;
        System* system;
        
        void checkModeChange();

    public:
        ClockMode(Clock* clock, System* sys);
        virtual ~ClockMode();

        virtual bool getTick() = 0;
    };
}