#pragma once

#include "task_visual.hpp"

#define MONITOR_WIDTH 100
#define COLUMN_PADDING 2

namespace UI
{
    class TaskInfo : public TaskVisual
    {
    private:
        map<string, string> MONITOR_LABELS;
        map<string, string> MONITOR_LABELS_STATUS;
        bool simulation_finished;
        double avg_turnaround;
        double avg_wait;

    public:
        TaskInfo();
        ~TaskInfo();
        
        void drawStaticInfo();
        void positionCorrectionForSimulation(int y_pos);
        void displayFinalStatistics(double avg_t, double avg_w);
        virtual void setTasks(vector<TCB *> *tasks, int y_offset = 0);
        virtual void drawTick(int tick);
    };
} // namespace UI