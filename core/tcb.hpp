#pragma once

#include <string>
#include <list>
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

    class TCB
    {
    private:
        const string id;
        const int color;
        const int start;
        const int duration;
        const int priority;
        int remaining;
        TCBState state;
        list<string> events;

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