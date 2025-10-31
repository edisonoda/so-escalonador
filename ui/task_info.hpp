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

    public:
        TaskInfo();
        ~TaskInfo();

        virtual void setTasks(vector<TCB *> *tasks, int y_offset = 0);
        void positionCorrectionForSimulation(int y_pos);
        void drawStaticInfo();
        virtual void drawTick(int tick);
    };
} // namespace UI