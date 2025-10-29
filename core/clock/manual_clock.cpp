#include "manual_clock.hpp"
#include "clock.hpp"
#include "../system.hpp"
#include "../../utils/std_libraries.hpp"

using namespace Core;

ManualClock::ManualClock(Clock* c, System* sys) : ClockMode(c, sys) { }

ManualClock::~ManualClock() { }

bool ManualClock::getTick()
{
    timeout(-1);
    char ch = screen->getCh();

    if (ch == ' ')
    {
        clock->selectMode('A');
        return false;
    }

    return true;
}
