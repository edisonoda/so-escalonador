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