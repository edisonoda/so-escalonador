#pragma once

#include "task_visual.hpp"

namespace UI
{
    class GanttChart : public TaskVisual
    {
    public:
        GanttChart();
        ~GanttChart();

        virtual void drawTick(int tick);
    };
} // namespace UI