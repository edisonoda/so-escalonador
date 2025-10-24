#include "manual_clock.hpp"
#include "../system.hpp"

using namespace Core;

ManualClock::ManualClock(System* sys, int total_t) : Clock(sys, total_t) { }

ManualClock::~ManualClock() { }

bool ManualClock::getTick()
{
    system->handleInterruption(Interruption::FULL_STOP);
    return true;
}
