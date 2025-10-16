#pragma once

#include <string>
#include <list>
using namespace std;

namespace Core
{
    class TCB
    {
    private:
        string id;
        string color;
        int start;
        int duration;
        int priority;
        int remaining;
        string state;
        list<string> events;

    public:
        TCB(string id, string color, int start, int duration, int priority, list<string> events = {});
        ~TCB();

        string getId();
        string getColor();
        int getStart();
        int getDuration();
        int getPriority();
        int getRemaining();
        string getState();
        
        void setState(string state);
        void decrementRemaining(int amount = 1);
    };
} // namespace Core