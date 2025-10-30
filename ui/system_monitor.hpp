#pragma once

#include "task_visual.hpp"

#define MONITOR_WIDTH 75

namespace UI
{
    class SystemMonitor : public TaskVisual
    {
    public:
        SystemMonitor();
        ~SystemMonitor();
        
        virtual void drawTick(int tick);
    };
} // namespace UI