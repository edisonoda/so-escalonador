#include "clock.hpp"

#include "../system.hpp"
#include "../../utils/constants.hpp"

using namespace Core;

Clock::Clock(System* sys, int total_t)
    : system(sys)
    , tick_interval(Constants::DEFAULT_TICK_INTERVAL)
    , quantum_interval(Constants::DEFAULT_QUANTUM)
{
    tick_counter = 0;
    total_time = total_t ? total_t : -1;
    quantum = 0;
    ticked = true;
    running = true;
    start_time = system_clock::now();
}

Clock::~Clock() {
    system = nullptr;
}

void Clock::run()
{
    while (running)
    {
        if (getTick())
        {
            total_time++;
            quantum++;

            if (quantum >= quantum_interval)
                system->handleInterruption(Interruption::QUANTUM);
            
            system->tick();
        }
    }
    
    system->handleInterruption(Interruption::FULL_STOP);
}

void Clock::stop()
{
    running = false;
}