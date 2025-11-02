#pragma once

#include "../utils/std_libraries.hpp"
#include "screen.hpp"
#include "../core/tcb.hpp"

using namespace Core;

namespace UI
{
    class TaskVisual : public Window
    {
    protected:
        vector<TCB *> *ord_tasks;
        Window *window;
        int visual_edge_x;
        int visual_edge_y;
        int x_offset;
        int y_offset;

        void checkEdges(int x, int y);
        
        public:
        TaskVisual();
        virtual ~TaskVisual();

        virtual void setTasks(vector<TCB *> *tasks, int y_offset = 0);
        
        virtual void print(int x, int y, string str);
        virtual void print(int x, int y, int ch);
        virtual void drawTick(int tick) = 0;
    };
} // namespace UI