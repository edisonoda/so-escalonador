#pragma once

#include <string>
#include <list>
using namespace std;

class TCB {
    private:
        int id;
        string color;
        int start;
        int duration;
        int priority;
        int remaining;
        string state;

    public:
        TCB(int id, string color, int start, int duration, int priority);
        ~TCB();

        int getId();
        string getColor();
        int getStart();
        int getDuration();
        int getPriority();
        int getRemaining();
        string getState();

        void setState(string state);
        void decrementRemaining();
};