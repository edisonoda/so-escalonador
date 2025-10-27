#include "gantt_chart.hpp"

using namespace UI;

#define PRINT_UNIT "   "

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

    for (size_t i = 0; i < ord_tasks->size(); i++)
    {
        TCB* task = (*ord_tasks)[i];
        int color;

        switch (task->getState())
        {
            case TCBState::READY:
                color = screen->setColor(DefaultColor::GRAY); // cinza
                break;
            case TCBState::RUNNING:
                color = screen->setColor(i); // cor da tarefa no fundo
                break;
            case TCBState::SUSPENDED:
                color = screen->setColor(DefaultColor::GRAY); // cinza
                break;
            default:
                color = screen->setColor(DefaultColor::BLACK); // preto
                break;
        }

        chart_generator->registerEntry(tick, i, color);
        print(x, i, PRINT_UNIT);
    }

    screen->setColor(DefaultColor::WHITE); // branco no preto
    print(x, ord_tasks->size(), to_string(tick));
}