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
}

void GanttChart::setTasks(vector<Core::TCB*>* tasks)
{
    ord_tasks = tasks;
    // imprime o eixo y
}