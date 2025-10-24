#include "system_clock.hpp"

#include "system.hpp"
#include "../utils/constants.hpp"

using namespace Core;

SystemClock::SystemClock(System* sys)
    : system(sys)
    , tick_interval(Constants::DEFAULT_TICK_INTERVAL)
    , quantum_interval(Constants::DEFAULT_QUANTUM)
{
    tick_counter = 0;
    total_time = -1;
    quantum = 0;
    ticked = true;
    running = true;
    start_time = system_clock::now();
}

SystemClock::~SystemClock() {
    system = nullptr;
}

bool SystemClock::getTick()
{
    end_time = system_clock::now();
    elapsed = end_time - start_time;
    int elap = (int) (elapsed.count() * 1000);

    ticked = false;

    if (elap >= tick_interval && elap > 0)
    {
        ticked = true;
        total_time++;
        quantum++;

        int t = (elap - (elap % tick_interval))/tick_interval;
		tick_counter += t;

		if (tick_counter >= 60)
			tick_counter = 0;

		start_time = end_time;
    }

    return ticked;
}

void SystemClock::run()
{
    while (running)
    {
        if (getTick())
        {
            if (quantum >= quantum_interval)
                system->handleInterruption(Interruption::QUANTUM);
            
            system->tick();
        }
    }
    
    system->handleInterruption(Interruption::FULL_STOP);
}

void SystemClock::stop()
{
    running = false;
}