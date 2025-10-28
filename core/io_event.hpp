#pragma once

#include "clock/tick_observer.hpp"

namespace Core { class TCB; }
namespace Core { class System; }
namespace Core { class Clock; }

namespace Core
{
    class IOEvent : public TickObserver
    {
    private:
        const int duration;
        int remaining_time;

        TCB* task;
        System* system;
        Clock* clock;
    
    public:
        IOEvent(TCB* task, System* system, Clock* clock, const int duration);
        ~IOEvent();

        virtual void tick();
    };
} // namespace Core