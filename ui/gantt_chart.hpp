#pragma once

#include "task_visual.hpp"
#include "../utils/gantt_exporter.hpp"

#define UNIT_WIDTH 3

namespace UI
{
    class GanttChart : public TaskVisual
    {
    private:
        Utils::GanttExporter *gantt_exporter;

    public:
        GanttChart(Utils::GanttExporter *chart_gen);
        ~GanttChart();

        virtual void setTasks(vector<TCB *> *tasks, int y_offset = 0);
        virtual void drawTick(int tick);
    };
} // namespace UI