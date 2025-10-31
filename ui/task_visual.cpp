#include "task_visual.hpp"

#include "window.hpp"

using namespace UI;

TaskVisual::TaskVisual() : Window()
{
    visual_edge_x = 0;
    visual_edge_y = 0;
    offset = 0;
    ord_tasks = nullptr;
}

TaskVisual::~TaskVisual()
{
    ord_tasks = nullptr;
}

void TaskVisual::setTasks(vector<TCB*>* tasks)
{
    int max_lenght = 0;
    ord_tasks = tasks;
    invertColor(true);

    // imprime o eixo y
    for (size_t i = 0; i < tasks->size(); i++)
    {
        TCB* task = (*tasks)[i];
        if (task->getId().length() > max_lenght)
            max_lenght = task->getId().length();
        setColor(i);
        print(0, i, task->getId());
    }

    offset = max_lenght + 1;

    setColor(DefaultColor::WHITE);
    invertColor(false);

    int x = visual_edge_x;
    for (size_t i = 0; i < ord_tasks->size(); i++)
        print(x, i, '|');
}


void TaskVisual::print(int x, int y, string str)
{
    checkEdges(x + str.length(), y);
    Window::print(x, y, str);
}

void TaskVisual::print(int x, int y, int ch)
{
    checkEdges(x + 1, y);
    Window::print(x, y, ch);
}

void TaskVisual::checkEdges(int x, int y)
{
    if (x > visual_edge_x)
        visual_edge_x = x;

    if (y > visual_edge_y)
        visual_edge_y = y;
}