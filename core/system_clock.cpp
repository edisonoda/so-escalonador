#include "system_clock.hpp"

#include "../utils/constants.hpp"

using namespace Core;

SystemClock::SystemClock() : tick_interval(Constants::DEFAULT_TICK_INTERVAL) {
    tick_counter = 0;
    ticked = true;
    start_time = system_clock::now();
}

SystemClock::~SystemClock() { }

bool SystemClock::getTick()
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

		if(tick_counter >= 60)
			tick_counter = 0;

		start_time = end_time;
    }

    return ticked;
}
