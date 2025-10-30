#pragma once

#include "../utils/std_libraries.hpp"
#include "window.hpp"
#include "../core/tcb.hpp"

using namespace Core;

namespace UI
{
    class TaskVisual : public Window
    {
    protected:
        vector<TCB*>* ord_tasks;
        Window* window;
        int visual_edge_x;
        int visual_edge_y;

        void checkEdges(int x, int y);
        virtual void print(int x, int y, string str);
        virtual void print(int x, int y, int ch);

    public:
        TaskVisual();
        ~TaskVisual();
        
        void setTasks(vector<TCB*>* tasks);
        virtual void drawTick(int tick) = 0;
    };
} // namespace UI