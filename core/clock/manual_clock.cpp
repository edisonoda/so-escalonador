#include "manual_clock.hpp"
#include "clock.hpp"
#include "../system.hpp"

using namespace Core;

ManualClock::ManualClock(Clock* c, System* sys) : ClockMode(c, sys) { }

ManualClock::~ManualClock() { }

bool ManualClock::getTick()
{
    // checkModeChange();
    int ch = screen->getCh();

    if (ch == ' ')
    {
        clock->selectMode('A');
        return false;
    }

    return true;
}
