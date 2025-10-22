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
        int screen_edge_x;
        int screen_edge_y;

        GanttChart();
        void checkScreenEdges(int x, int y);
        void print(int x, int y, string str);
        void print(int x, int y, char ch);

    public:
        static GanttChart* getInstance();
        void setScreen(Screen* scr) { screen = scr; }
        void drawTick(int tick);
        void setTasks(vector<TCB*>* tasks);

        ~GanttChart();
    };
} // namespace UI