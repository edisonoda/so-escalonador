#pragma once

#include "task_visual.hpp"
#include "../utils/chart_generator.hpp"

#define UNIT_WIDTH 3

namespace UI
{
    class GanttChart : public TaskVisual
    {
    private:
        Utils::ChartGenerator *chart_generator;

    public:
        GanttChart(Utils::ChartGenerator *chart_gen);
        ~GanttChart();

        virtual void setTasks(vector<TCB *> *tasks, int y_offset = 0);
        virtual void drawTick(int tick);
    };
} // namespace UI