#include "task_info.hpp"

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
        {"Start", "START: "},
        {"Duration", "DURATION: "},
        {"Remaining", "REMAINING: "},
        {"Priority", "PRIORITY: "},
        {"Color", "COLOR: "}
    };

    MONITOR_LABELS_STATUS = {
        {"New", "NEW        "},
        {"Ready", "READY      "},
        {"Running", "RUNNING    "},
        {"Suspended", "SUSPENDED  "},
        {"Terminated", "TERMINATED "},
        {"Error", "ERROR      "}
    };

    simulation_finished = false;
    avg_turnaround = 0.0;
    avg_wait = 0.0;
}

TaskInfo::~TaskInfo()
{
}


void TaskInfo::drawStaticInfo()
{
    int data_1_x = x_offset + 2;
    int bar_2_x = data_1_x + 11;
    int data_2_x = bar_2_x + 2;
    int bar_3_x = data_2_x + 12;
    int data_3_x = bar_3_x + 2;
    int bar_4_x = data_3_x + 15;
    int data_4_x = bar_4_x + 2;
    
    if (!ord_tasks)
    return;
    
    for (size_t i = 0; i < ord_tasks->size(); i++)
    {
        TCB *task = (*ord_tasks)[i];
        
        string color_str = MONITOR_LABELS.at("Color") + to_string(task->getColor());
        string start_str = MONITOR_LABELS.at("Start") + to_string(task->getStart());
        string duration_str = MONITOR_LABELS.at("Duration") + to_string(task->getDuration());
        string prio_str = MONITOR_LABELS.at("Priority") + to_string(task->getPriority());
        
        setColor(DefaultColor::WHITE);
        invertColor(false);
        
        print(data_1_x, i + y_offset, color_str);
        
        print(bar_2_x, i + y_offset, "|");
        print(data_2_x, i + y_offset, start_str);
        
        print(bar_3_x, i + y_offset, "|");
        print(data_3_x, i + y_offset, duration_str);
        
        print(bar_4_x, i + y_offset, "|");
        print(data_4_x, i + y_offset, prio_str);
    }
}

void TaskInfo::positionCorrectionForSimulation(int y_pos)
{
    setWindowDimensions(
        (ord_tasks ? ord_tasks->size() : 5) + 3,
        MONITOR_WIDTH,
        0,
        y_pos
    );
}

void TaskInfo::displayFinalStatistics(double avg_t, double avg_w)
{
    this->avg_turnaround = avg_t;
    this->avg_wait = avg_w;
    this->simulation_finished = true;
    
    // Força um último "tick" para redesenhar a janela
    // com as estatísticas finais
    drawTick(0);
}

void TaskInfo::setTasks(vector<TCB *> *tasks, int y_offset)
{
    TaskVisual::setTasks(tasks, y_offset);
}

void TaskInfo::drawTick(int tick)
{
    int data_1_x = x_offset + 2;
    int bar_2_x = data_1_x + 12;
    int data_2_x = bar_2_x + 2;
    int bar_3_x = data_2_x + 12;
    int data_3_x = bar_3_x + 2;
    int bar_4_x = data_3_x + 15;
    int data_4_x = bar_4_x + 2;
    int bar_5_x = data_4_x + 16;
    int data_5_x = bar_5_x + 2;

    for (size_t i = 0; i < ord_tasks->size(); i++)
    {
        TCB *task = (*ord_tasks)[i];

        setColor(i);
        invertColor(true);

        TCBState status = task->getState();
        string status_str;
        string start_str = MONITOR_LABELS.at("Start") + to_string(task->getStart());
        string duration_str = MONITOR_LABELS.at("Duration") + to_string(task->getDuration());
        string rem_str = MONITOR_LABELS.at("Remaining") + to_string(task->getRemaining());
        string prio_str = MONITOR_LABELS.at("Priority") + to_string(task->getPriority());

        switch (status)
        {
        case TCBState::NEW:
            status_str = MONITOR_LABELS_STATUS.at("New");
            break;
        case TCBState::READY:
            status_str = MONITOR_LABELS_STATUS.at("Ready");
            break;
        case TCBState::RUNNING:
            invertColor(false);
            status_str = MONITOR_LABELS_STATUS.at("Running");
            break;
        case TCBState::SUSPENDED:
            status_str = MONITOR_LABELS_STATUS.at("Suspended");
            break;
        case TCBState::TERMINATED:
            status_str = MONITOR_LABELS_STATUS.at("Terminated");
            break;
        default:
            status_str = MONITOR_LABELS_STATUS.at("Error");
            break;
        }

        setColor(DefaultColor::WHITE);
        invertColor(false);

        setColor(i); 
        invertColor(true);
        if (status == TCBState::RUNNING) 
            invertColor(false);
        print(data_1_x, i + y_offset, status_str);
        
        setColor(DefaultColor::WHITE); invertColor(false);
        print(bar_2_x, i + y_offset, "|");
        print(data_2_x, i + y_offset, start_str);
        print(bar_3_x, i + y_offset, "|");
        print(data_3_x, i + y_offset, duration_str);
        print(bar_4_x, i + y_offset, "|");
        print(data_4_x, i + y_offset, rem_str);
        print(bar_5_x, i + y_offset, "|");
        print(data_5_x, i + y_offset, prio_str);
    }

    if (simulation_finished)
    {
        setColor(DefaultColor::WHITE);
        invertColor(false);
        
        // Posição Y: 1 linha abaixo da última task
        int stats_y_pos = ord_tasks->size() + 1; 
        
        string turn_str = "Tempo de Vida Médio (Turnaround): " + to_string_with_precision(avg_turnaround);
        string wait_str = "Tempo de Espera Médio:           " + to_string_with_precision(avg_wait);

        print(1, stats_y_pos, turn_str);
        print(1, stats_y_pos + 1, wait_str);
    }

    refresh();
}