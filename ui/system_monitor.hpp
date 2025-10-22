#pragma once

#include <vector>
#include <string>
#include "screen.hpp"
#include "../core/tcb.hpp"

using namespace std;
using namespace Core;

namespace UI
{
    class SystemMonitor
    {
    private:
        static SystemMonitor *instance;
        Screen *screen;
        vector<TCB *> *ord_tasks;

        SystemMonitor();

    public:
        static SystemMonitor *getInstance();
        void setScreen(Screen *scr) { screen = scr; }
        void setTasks(vector<TCB *> *tasks);
        void draw();

        ~SystemMonitor();
    };
} // namespace UI