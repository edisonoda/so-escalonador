#pragma once

#include <vector>
#include "../core/tcb.hpp"
#include "screen.hpp"

using namespace std;
using namespace Core;

namespace UI
{
    class GanttChart
    {
    private:
        static GanttChart* instance;
        vector<TCB*>* ord_tasks; 
        Screen* screen;

        GanttChart();

    public:
        static GanttChart* getInstance();
        void setScreen(Screen* scr) { screen = scr; }
        void draw(int tick);
        void setTasks(vector<TCB*>* tasks);

        ~GanttChart();
    };
} // namespace UI