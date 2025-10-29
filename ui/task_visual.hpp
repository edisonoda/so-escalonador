#pragma once

#include "../utils/std_libraries.hpp"
#include "../core/tcb.hpp"
#include "screen.hpp"

using namespace Core;

namespace UI
{
    class TaskVisual
    {
    protected:
        vector<TCB*>* ord_tasks;
        Screen* screen;
        int visual_edge_x;
        int visual_edge_y;
        int offset;

        void checkEdges(int x, int y);
        void print(int x, int y, string str);
        void print(int x, int y, char ch);

    public:
        TaskVisual();
        ~TaskVisual();

        void setScreen(Screen* scr) { screen = scr; }
        void setTasks(vector<TCB*>* tasks);
        void setOffset(const int off);
        virtual void drawTick(int tick) = 0;
    };
} // namespace UI