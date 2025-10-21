#pragma once

#include <vector>
#include "../core/tcb.hpp"
#include "screen.hpp"
using namespace std;

namespace UI
{
    class GanttChart
    {
    private:
        static GanttChart* instance;
        vector<Core::TCB*>* ord_tasks; 
        Screen* screen;

        GanttChart();

    public:
        static GanttChart* getInstance();
        void draw(int tick);
        void setTasks(vector<Core::TCB*>* tasks);

        ~GanttChart();
    };
} // namespace UI