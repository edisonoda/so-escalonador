#pragma once

#include "task_visual.hpp"

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