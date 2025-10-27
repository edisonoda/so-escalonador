#pragma once

#include "../../ui/screen.hpp"

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
        UI::Screen* screen;
        
        void checkModeChange();

    public:
        ClockMode(Clock* clock, System* sys);
        virtual ~ClockMode();

        virtual bool getTick() = 0;
    };
}