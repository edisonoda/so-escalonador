#include "gantt_chart.hpp"
#include "screen.hpp"
using namespace UI;

#define PRINT_UNIT "   "

GanttChart* GanttChart::instance = nullptr;

GanttChart::GanttChart()
{
    screen_edge_x = 0;
    screen_edge_y = 0;
}

GanttChart::~GanttChart()
{
    instance = nullptr;
}

GanttChart* GanttChart::getInstance()
{
    if (instance == nullptr)
        instance = new GanttChart();
    
    return instance;
}

void GanttChart::drawTick(int tick)
{
    // um tick = três colunas
    // vamos desenhar coluna por coluna
    int x = screen_edge_x;

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

    screen->refresh();
}

void GanttChart::setTasks(vector<TCB*>* tasks)
{
    ord_tasks = tasks;
    screen->invertColor(true);
    // imprime o eixo y
    for (size_t i = 0; i < ord_tasks->size(); i++)
    {
        TCB* task = (*ord_tasks)[i];
        screen->setColor(i);
        print(0, i, task->getId());
        print(screen_edge_x, i, '|');
    }
    screen->invertColor(false);
    screen->refresh();
}

void GanttChart::print(int x, int y, string str)
{
    checkScreenEdges(x + str.length(), y);
    screen->print(x, y, str);
}

void GanttChart::print(int x, int y, char ch)
{
    checkScreenEdges(x, y);
    screen->print(x, y, ch);
}

void GanttChart::checkScreenEdges(int x, int y)
{
    if (x > screen_edge_x)
        screen_edge_x = x;

    if (y > screen_edge_y)
        screen_edge_y = y;
}