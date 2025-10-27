#pragma once

#include "task_visual.hpp"
#include "../utils/chart_generator.hpp"

namespace UI
{
    class GanttChart : public TaskVisual
    {
    private:
        Utils::ChartGenerator *chart_generator;

    public:
        GanttChart(Utils::ChartGenerator *chart_gen);
        ~GanttChart();

        virtual void drawTick(int tick);
    };
} // namespace UI