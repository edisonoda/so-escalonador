#pragma once

#include <string>
#include <list>
#include <map>
using namespace std;

namespace Core
{
    enum class TCBState
    {
        NEW,
        READY,
        RUNNING,
        SUSPENDED,
        TERMINATED
    };

    enum class EventType
    {
        IO,
        ML,
        MU
    };

    struct Event
    {
        EventType type;
        int start;
        int duration;
    };

    class TCB
    {
    private:
        const static map<string, EventType> events_map;

        const string id;
        const int color;
        const int start;
        const int duration;
        const int priority;
        int remaining;
        TCBState state;
        list<Event> events;

        void createEvent(string ev);

    public:
        TCB(string id, int color, int start, int duration, int priority, list<string> events = {});
        ~TCB();

        string getId() const;
        int getColor() const;
        int getStart() const;
        int getDuration() const;
        int getPriority() const;
        int getRemaining() const;
        TCBState getState() const;

        void setState(TCBState state);
        void decrementRemaining(int amount = 1);
    };
} // namespace Core