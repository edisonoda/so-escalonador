#include "task_visual.hpp"

#include "window.hpp"

using namespace UI;

TaskVisual::TaskVisual() : Window()
{
    visual_edge_x = 0;
    visual_edge_y = 0;
    x_offset = 0;
    y_offset = 0;
    ord_tasks = nullptr;
}

TaskVisual::~TaskVisual()
{
    ord_tasks = nullptr;
}

void TaskVisual::setTasks(vector<TCB*>* tasks, int y_offset)
{
    ord_tasks = tasks;
    this->y_offset = y_offset;
    int max_lenght = 0;
    invertColor(true);

    for (size_t i = 0; i < tasks->size(); i++)
    {
        TCB* task = (*tasks)[i];
        string id_str = task->getId();
        if (id_str.length() >= max_lenght)
        {
            max_lenght = id_str.length(); 
        }

        setColor(i);
        
        print(0, i + y_offset, id_str);
    }

    x_offset = max_lenght + 1;

    setColor(DefaultColor::WHITE);
    invertColor(false);

    for (size_t i = 0; i < tasks->size(); i++)
        print(x_offset, i + y_offset, '|');
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