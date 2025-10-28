#include "io_event.hpp"
#include "system.hpp"
#include "clock/clock.hpp"

using namespace Core;

IOEvent::IOEvent(TCB* task, System* sys, Clock* clock, const int d) : TickObserver()
    , task(task)
    , system(sys)
    , clock(clock)
    , duration(d)
{
    remaining_time = duration;
    clock->attach(this);
}

IOEvent::~IOEvent()
{
    clock->detach(this);
    task = nullptr;
    system = nullptr;
    clock = nullptr;
}

void IOEvent::tick()
{
    remaining_time--;

    if (remaining_time <= 0)
        system->handleInterruption(Interruption::FINISH_IO, task);
}