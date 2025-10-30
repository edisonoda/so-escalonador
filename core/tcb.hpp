#pragma once

#include "../utils/std_libraries.hpp"

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
        string id;
        int color;
        int start;
        int duration;
        int priority;
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

        void setId(const string _id);
        void setColor(const int _color);
        void setStart(const int _start);
        void setDuration(const int _duration);
        void setPriority(const int _priority);

        int getRemaining() const;
        TCBState getState() const;

        void setState(TCBState state);
        void decrementRemaining(int amount = 1);
    };
} // namespace Core