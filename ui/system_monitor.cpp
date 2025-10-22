#include "system_monitor.hpp"
#include <iostream>

using namespace std;

using namespace UI;

SystemMonitor *SystemMonitor::instance = nullptr;

SystemMonitor::SystemMonitor()
{
}

SystemMonitor::~SystemMonitor()
{
    instance = nullptr;
}

SystemMonitor *SystemMonitor::getInstance()
{
    if (instance == nullptr)
        instance = new SystemMonitor();

    return instance;
}

void SystemMonitor::draw()
{
    int x = 4;

    for (size_t i = 0; i < ord_tasks->size(); i++)
    {
        TCB *task = (*ord_tasks)[i];

        screen->setColor(DefaultColor::BLACK);

        screen->setColor(i); // Cor da tarefa
        TCBState status = task->getState();
        string status_str;
        string rem_str = " REMAINING: " + to_string(task->getRemaining()) + " ";

        switch (status)
        {
        case TCBState::READY:
            status_str = "READY     ";
            break;
        case TCBState::RUNNING:
            status_str = "RUNNING   ";
            break;
        case TCBState::SUSPENDED:
            status_str = "SUSPENDED ";
            break;
        case TCBState::TERMINATED:
            status_str = "TERMINATED";
            break;
        default:
            status_str = "NEW       ";
            break;
        }

        screen->sprint(x, i, status_str);
        
        screen->setColor(DefaultColor::WHITE);
        screen->sprint(x + 10, i, rem_str);
    }

    screen->srefresh();
}

void SystemMonitor::setTasks(vector<TCB *> *tasks)
{
    ord_tasks = tasks;
    screen->invertColor(true);
    // imprime o eixo y
    for (size_t i = 0; i < ord_tasks->size(); i++)
    {
        TCB *task = (*ord_tasks)[i];
        screen->setColor(i);
        screen->sprint(0, i, task->getId());
        screen->sprint(3, i, '|');
    }
    screen->invertColor(false);
    screen->srefresh();
}