#include "task_info.hpp"

#define INFO_SPACE 4

using namespace UI;

static string to_string_with_precision(double val, int precision = 2)
{
    stringstream stream;
    stream << fixed << setprecision(precision) << val;
    return stream.str();
}

TaskInfo::TaskInfo() : TaskVisual()
{
    MONITOR_LABELS = {
        {"Color",       "COLOR: "},
        {"Start",       "START: "},
        {"Duration",    "DURATION: "},
        {"Priority",    "PRIORITY: "},
        {"Remaining",   "REMAINING: "}
    };

    MONITOR_LABELS_STATUS = {
        {"New",         "NEW        "},
        {"Ready",       "READY      "},
        {"Running",     "RUNNING    "},
        {"Suspended",   "SUSPENDED  "},
        {"Terminated",  "TERMINATED "},
        {"Error",       "ERROR      "}
    };

    avg_turnaround = 0.0;
    avg_wait = 0.0;
}

TaskInfo::~TaskInfo()
{
}

void TaskInfo::setTasks(vector<TCB *> *tasks, int y_offset)
{
    TaskVisual::setTasks(tasks, y_offset);

    int width = MONITOR_LABELS_STATUS["Terminated"].length() + INFO_SPACE;

    for (auto label : MONITOR_LABELS)
        width += label.second.length() + INFO_SPACE;

    setWindowDimensions(
        tasks->size() + y_offset,
        width + x_offset,
        0,
        tasks->size() + 3
    );
}

void TaskInfo::drawTick(int tick)
{
    int x;

    for (size_t i = 0; i < ord_tasks->size(); i++)
    {
        TCB *task = (*ord_tasks)[i];
        x = x_offset;

        setColor(i);
        invertColor(true);

        TCBState status = task->getState();
        string status_str;
        string rem_str = MONITOR_LABELS["Remaining"] + to_string(task->getRemaining());

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

        print(x, i + y_offset, status_str);
        invertColor(true);
        print(x = x + INFO_SPACE + MONITOR_LABELS_STATUS["Terminated"].length(), i + y_offset, rem_str);
        
        drawStaticInfo(i, x + INFO_SPACE + MONITOR_LABELS["Remaining"].length() - x_offset);
    }

    refresh();
}

void TaskInfo::drawStaticInfo(int i, int offset)
{
    if (!ord_tasks)
        return;
    
    int x = x_offset + offset;
    TCB *task = (*ord_tasks)[i];
        
    string color_str = MONITOR_LABELS["Color"] + to_string(task->getColor());
    string start_str = MONITOR_LABELS["Start"] + to_string(task->getStart());
    string duration_str = MONITOR_LABELS["Duration"] + to_string(task->getDuration());
    string prio_str = MONITOR_LABELS["Priority"] + to_string(task->getPriority());
    
    setColor(DefaultColor::WHITE);
    invertColor(false);
    
    print(x, i + y_offset, color_str);
    print(x = x + INFO_SPACE + MONITOR_LABELS["Color"].length(), i + y_offset, start_str);
    print(x = x + INFO_SPACE + MONITOR_LABELS["Start"].length(), i + y_offset, duration_str);
    print(x = x + INFO_SPACE + MONITOR_LABELS["Duration"].length(), i + y_offset, prio_str);
}

void TaskInfo::displayFinalStatistics(double avg_t, double avg_w)
{
    this->avg_turnaround = avg_t;
    this->avg_wait = avg_w;
    
    // Força um último "tick" para redesenhar a janela com as estatísticas finais
    drawTick(0);

    setColor(DefaultColor::WHITE);
    invertColor(false);
    
    // Posição Y: 1 linha abaixo da última task
    int stats_y_pos = y_offset + ord_tasks->size() + 1; 
    
    string turn_str = "Tempo de Vida Médio:\t" + to_string_with_precision(avg_turnaround);
    string wait_str = "Tempo de Espera Médio:\t" + to_string_with_precision(avg_wait);

    print(1, stats_y_pos, turn_str);
    print(1, stats_y_pos + 1, wait_str);
}