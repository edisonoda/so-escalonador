#pragma once

#include "task_visual.hpp"

#define MONITOR_WIDTH 75

namespace UI
{
    class SystemMonitor : public TaskVisual
    {
    private:
        map<string, string> MONITOR_LABELS;
        map<string, string> MONITOR_LABELS_STATUS;

    public:
        SystemMonitor();
        ~SystemMonitor();
        
        virtual void setTasks(vector<TCB *> *tasks);
        virtual void drawTick(int tick);
    };
} // namespace UI