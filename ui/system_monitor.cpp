#include "system_monitor.hpp"

using namespace std;

using namespace UI;

SystemMonitor *SystemMonitor::instance = nullptr;

SystemMonitor::SystemMonitor()
{
    offset = 0;
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

        screen->print(x, i + offset, status_str);
        
        screen->setColor(DefaultColor::WHITE);
        screen->print(x + 10, i + offset, rem_str);
    }

    screen->refresh();
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
        screen->print(0, i + offset, task->getId());
        screen->print(task->getId().length(), i + offset, "|");
    }
    screen->invertColor(false);
    screen->refresh();
}

void SystemMonitor::setOffset(const int off)
{
    offset = off;
}