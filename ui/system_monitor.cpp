#include "system_monitor.hpp"

#define SPACE_BETWEEN 4

using namespace UI;

SystemMonitor::SystemMonitor() : TaskVisual()
{
    MONITOR_LABELS = {
        {"Start", " | START: "},
        {"Duration", " | DURATION: "},
        {"Remaining", " | REMAINING: "},
        {"Priority", " | PRIORITY: "}
    };

    MONITOR_LABELS_STATUS = {
        {"New", " NEW       "},
        {"Ready", " READY     "},
        {"Running", " RUNNING   "},
        {"Suspended", " SUSPENDED "},
        {"Terminated", " TERMINATED"},
        {"Error", " ERROR     "}
    };
}

SystemMonitor::~SystemMonitor()
{
}

void SystemMonitor::drawTick(int tick)
{
    int x = offset;

    for (size_t i = 0; i < ord_tasks->size(); i++)
    {
        TCB *task = (*ord_tasks)[i];

        x = offset;

        setColor(i); // Cor da tarefa
        invertColor(true);
        
        TCBState status = task->getState();
        string status_str;
        string start_str = MONITOR_LABELS["Start"] + to_string(task->getStart());
        string duration_str = MONITOR_LABELS["Duration"] + to_string(task->getDuration());
        string rem_str = MONITOR_LABELS["Remaining"] + to_string(task->getRemaining());
        string prio_str = MONITOR_LABELS["Priority"] + to_string(task->getPriority());

        switch (status)
        {
            case TCBState::NEW:
                status_str = MONITOR_LABELS_STATUS["New"];
                break;
            case TCBState::READY:
                status_str = MONITOR_LABELS_STATUS["Ready"];
                break;
            case TCBState::RUNNING:
                invertColor(false);
                status_str = MONITOR_LABELS_STATUS["Running"];
                break;
            case TCBState::SUSPENDED:
                status_str = MONITOR_LABELS_STATUS["Suspended"];
                break;
            case TCBState::TERMINATED:
                status_str = MONITOR_LABELS_STATUS["Terminated"];
                break;
            default:
                status_str = MONITOR_LABELS_STATUS["Error"];
                break;
        }

        print(x, i, status_str);
        
        setColor(DefaultColor::WHITE);
        invertColor(false);
        
        print(x = x + SPACE_BETWEEN + MONITOR_LABELS_STATUS["Terminated"].length(), i, start_str);
        print(x = x + SPACE_BETWEEN + MONITOR_LABELS["Start"].length(), i, duration_str);
        print(x = x + SPACE_BETWEEN + MONITOR_LABELS["Duration"].length(), i, rem_str);
        print(x = x + SPACE_BETWEEN + MONITOR_LABELS["Remaining"].length(), i, prio_str);
    }

    refresh();
}

void SystemMonitor::setTasks(vector<TCB *> *tasks)
{
    TaskVisual::setTasks(tasks);

    int width = MONITOR_LABELS_STATUS["Terminated"].length() + SPACE_BETWEEN;

    for (auto label : MONITOR_LABELS)
        width += label.second.length() + SPACE_BETWEEN;

    setWindowDimensions(
        tasks->size(),
        width + offset,
        0,
        tasks->size() + 3
    );
}