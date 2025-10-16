#pragma once

#include <string>
#include <list>
using namespace std;

namespace Core
{
    class TCB
    {
    private:
        const string id;
        const string color;
        const int start;
        const int duration;
        const int priority;
        int remaining;
        string state;
        list<string> events;

    public:
        TCB(string id, string color, int start, int duration, int priority, list<string> events = {});
        ~TCB();

        string getId() const;
        string getColor() const;
        int getStart() const;
        int getDuration() const;
        int getPriority() const;
        int getRemaining() const;
        string getState() const;
        
        void setState(string state);
        void decrementRemaining(int amount = 1);
    };
} // namespace Core