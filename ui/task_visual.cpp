#include "task_visual.hpp"
#include "screen.hpp"

using namespace UI;

TaskVisual::TaskVisual()
{
    visual_edge_x = 0;
    visual_edge_y = 0;
    offset = 0;
    ord_tasks = nullptr;
    screen = nullptr;
}

TaskVisual::~TaskVisual()
{
    ord_tasks = nullptr;
    screen = nullptr;
}

void TaskVisual::setTasks(vector<TCB*>* tasks)
{
    ord_tasks = tasks;
    screen->invertColor(true);

    // imprime o eixo y
    for (size_t i = 0; i < ord_tasks->size(); i++)
    {
        TCB* task = (*ord_tasks)[i];
        screen->setColor(i);
        print(0, i, task->getId());
    }

    screen->setColor(DefaultColor::WHITE);
    screen->invertColor(false);

    int x = visual_edge_x;
    for (size_t i = 0; i < ord_tasks->size(); i++)
        print(x, i, '|');
}

void TaskVisual::setOffset(const int off)
{
    offset = off;
}

void TaskVisual::print(int x, int y, string str)
{
    checkEdges(x + str.length(), y);
    screen->print(x, y + offset, str);
    screen->refresh();
}

void TaskVisual::print(int x, int y, char ch)
{
    checkEdges(x + 1, y);
    screen->print(x, y + offset, ch);
    screen->refresh();
}

void TaskVisual::checkEdges(int x, int y)
{
    if (x > visual_edge_x)
        visual_edge_x = x;

    if (y > visual_edge_y)
        visual_edge_y = y;
}