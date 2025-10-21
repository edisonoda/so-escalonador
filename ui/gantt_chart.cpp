#include "gantt_chart.hpp"
using namespace UI;

GanttChart* GanttChart::instance = nullptr;

GanttChart::GanttChart()
{
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

void GanttChart::draw(int tick)
{
    // um tick = três colunas
    // vamos desenhar coluna por coluna
    int x = screen->getScreenEdgeX();

    for (size_t i = 0; i < ord_tasks->size(); i++)
    {
        Core::TCB* task = (*ord_tasks)[i];

        switch (task->getState())
        {
            case Core::TCBState::READY:
                screen->setColor(3); // cinza
                break;
            case Core::TCBState::RUNNING:
                screen->setColor(i + 4); // cor da tarefa no fundo
                break;
            case Core::TCBState::SUSPENDED:
                screen->setColor(3); // cinza
                break;
            default:
                screen->setColor(2); // preto
                break;
        }

        screen->sprint(x, i, "   ");
    }

    screen->setColor(1); // branco no preto
    screen->sprint(x, ord_tasks->size(), to_string(tick));

    screen->srefresh();
}

void GanttChart::setTasks(vector<Core::TCB*>* tasks)
{
    ord_tasks = tasks;
    // imprime o eixo y
    for (size_t i = 0; i < ord_tasks->size(); i++)
    {
        Core::TCB* task = (*ord_tasks)[i];
        screen->sprint(0, i, task->getId());
        screen->sprint(screen->getScreenEdgeX(), i, '|');     
    }
    screen->srefresh();
}