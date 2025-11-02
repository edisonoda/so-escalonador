#pragma once

#include "task_visual.hpp"

namespace UI
{
    class TaskInfo : public TaskVisual
    {
    private:
        map<string, string> MONITOR_LABELS;
        map<string, string> MONITOR_LABELS_STATUS;

    public:
        TaskInfo();
        ~TaskInfo();
        
        void drawStaticInfo(int index, int offset = 0);
        void calcFinalStatistics(double *avg_turnaround, double *avg_wait);
        void displayFinalStatistics();

        virtual void setTasks(vector<TCB *> *tasks, int y_offset = 0);
        virtual void drawTick(int tick);
    };
} // namespace UI