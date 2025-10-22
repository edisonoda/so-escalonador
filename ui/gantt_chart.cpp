#include "gantt_chart.hpp"
#include "screen.hpp"
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

        screen->sprint(x, i, "   ");
    }

    screen->setColor(DefaultColor::WHITE); // branco no preto
    screen->sprint(x, ord_tasks->size(), to_string(tick));

    screen->srefresh();
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
        screen->sprint(28, i, task->getId());
        screen->sprint(31, i, '|');
        // screen->sprint(screen->getScreenEdgeX(), i, '|');     
    }
    screen->invertColor(false);
    screen->srefresh();
}