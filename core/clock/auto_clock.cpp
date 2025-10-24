#include "auto_clock.hpp"
#include "../system.hpp"

using namespace Core;

AutoClock::AutoClock(System* sys, int total_t) : Clock(sys, total_t) { }

AutoClock::~AutoClock() { }

bool AutoClock::getTick()
{
    end_time = system_clock::now();
    elapsed = end_time - start_time;
    int elap = (int) (elapsed.count() * 1000);

    ticked = false;

    if (elap >= tick_interval && elap > 0)
    {
        ticked = true;

        int t = (elap - (elap % tick_interval))/tick_interval;
		tick_counter += t;

		if (tick_counter >= 60)
			tick_counter = 0;

		start_time = end_time;
    }

    return ticked;
}