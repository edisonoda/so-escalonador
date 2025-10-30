#include "gantt_chart.hpp"

using namespace UI;

GanttChart::GanttChart(Utils::ChartGenerator *chart_gen) : TaskVisual() 
{
    chart_generator = chart_gen;
}

GanttChart::~GanttChart() 
{
    chart_generator = nullptr;
}

void GanttChart::drawTick(int tick)
{
    // um tick = três colunas
    // vamos desenhar coluna por coluna
    int x = visual_edge_x;
    string unit = string(UNIT_WIDTH, ' ');

    for (size_t i = 0; i < ord_tasks->size(); i++)
    {
        TCB* task = (*ord_tasks)[i];
        int color;

        switch (task->getState())
        {
            case TCBState::RUNNING: color = setColor(i); break;
            case TCBState::READY: color = setColor(DefaultColor::GRAY); break;
            case TCBState::SUSPENDED: color = setColor(DefaultColor::GRAY); break;
            default: color = setColor(DefaultColor::BLACK); break;
        }

        chart_generator->registerEntry(tick, i, color);
        print(x, i, unit);
    }

    setColor(DefaultColor::WHITE); // branco no preto
    print(x, ord_tasks->size(), to_string(tick));
    refresh();
}