#include "auto_clock.hpp"
#include "clock.hpp"
#include "../system.hpp"
#include "../../utils/constants.hpp"
#include <ncurses.h>

using namespace Core;

AutoClock::AutoClock(Clock* c, System* sys) : ClockMode(c, sys)
    , tick_interval(Constants::DEFAULT_TICK_INTERVAL)
{
    tick_counter = 0;
    ticked = false;
    start_time = system_clock::now();
}

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

    timeout(0);
    int ch = screen->getCh();

    if (ch == ' ')
    {
        clock->selectMode('P');
        ticked = false;
    }
    
    return ticked;
}