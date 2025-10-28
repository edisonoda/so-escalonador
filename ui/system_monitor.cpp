#include "system_monitor.hpp"

using namespace UI;

SystemMonitor::SystemMonitor() : TaskVisual() { }

SystemMonitor::~SystemMonitor() { }

void SystemMonitor::drawTick(int tick)
{
    int x = 4;

    for (size_t i = 0; i < ord_tasks->size(); i++)
    {
        TCB *task = (*ord_tasks)[i];

        screen->setColor(task->getColor()); // Cor da tarefa
        screen->invertColor(true);
        
        TCBState status = task->getState();
        string status_str;
        string start_str = " START: " + to_string(task->getStart()) + " ";
        string duration_str = "| DURATION: " + to_string(task->getDuration()) + " ";
        string rem_str = "| REMAINING: " + to_string(task->getRemaining()) + " ";
        string prio_str = "| PRIORITY: " + to_string(task->getPriority()) + " ";

        switch (status)
        {
            case TCBState::NEW:
                status_str = "NEW       ";
                break;
            case TCBState::READY:
                status_str = "READY     ";
                break;
            case TCBState::RUNNING:
                screen->invertColor(false);
                status_str = "RUNNING   ";
                break;
            case TCBState::SUSPENDED:
                status_str = "SUSPENDED ";
                break;
            case TCBState::TERMINATED:
                status_str = "TERMINATED";
                break;
            default:
                status_str = "ERROR     ";
                break;
        }

        print(x, i, status_str);
        
        screen->setColor(DefaultColor::WHITE);
        screen->invertColor(false);
        
        print(x + 10, i, start_str);
        print(x + 20, i, duration_str);
        print(x + 35, i, rem_str);
        print(x + 50, i, prio_str);
    }
}