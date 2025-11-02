#pragma once

#include "task_visual.hpp"

namespace UI
{
    class TaskInfo : public TaskVisual
    {
    private:
        map<string, string> MONITOR_LABELS;
        map<string, string> MONITOR_LABELS_STATUS;

        double avg_turnaround;
        double avg_wait;

    public:
        TaskInfo();
        ~TaskInfo();
        
        void drawStaticInfo(int index, int offset = 0);
        void displayFinalStatistics(double avg_t, double avg_w);

        virtual void setTasks(vector<TCB *> *tasks, int y_offset = 0);
        virtual void drawTick(int tick);
    };
} // namespace UI