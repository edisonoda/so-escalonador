#include "gantt_chart.hpp"

using namespace UI;

#define PRINT_UNIT "   "

GanttChart::GanttChart() : TaskVisual() { }

GanttChart::~GanttChart() { }

void GanttChart::drawTick(int tick)
{
    // um tick = três colunas
    // vamos desenhar coluna por coluna
    int x = visual_edge_x;

    for (size_t i = 0; i < ord_tasks->size(); i++)
    {
        TCB* task = (*ord_tasks)[i];

        switch (task->getState())
        {
            case TCBState::READY:
                screen->setColor(DefaultColor::GRAY); // cinza
                break;
            case TCBState::RUNNING:
                screen->setColor(i); // cor da tarefa no fundo
                break;
            case TCBState::SUSPENDED:
                screen->setColor(DefaultColor::GRAY); // cinza
                break;
            default:
                screen->setColor(DefaultColor::BLACK); // preto
                break;
        }

        print(x, i, PRINT_UNIT);
    }

    screen->setColor(DefaultColor::WHITE); // branco no preto
    print(x, ord_tasks->size(), to_string(tick));
}