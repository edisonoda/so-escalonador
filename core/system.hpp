#pragma once

#include "../utils/std_libraries.hpp"

#include "tcb.hpp"
#include "clock.hpp"
#include "setup_manager.hpp"

#include "scheduler.hpp"

#include "../ui/screen.hpp"
#include "../ui/gantt_chart.hpp"
#include "../ui/task_info.hpp"

#include "../utils/chart_generator.hpp"

namespace Core
{
    enum class Interruption
    {
        QUANTUM,
        FULL_STOP
    };

    class System
    {
    private:
        static System* instance;
        
        Scheduler* scheduler;
        Clock clock;
        TCB* current_task;
        vector<TCB*> ord_tasks;
        list<TCB*> new_list;
        list<TCB*> ready_list;
        list<TCB*> suspended_list;

        Screen* screen;
        GanttChart gantt_chart;
        TaskInfo task_info;

        Utils::ChartGenerator chart_generator;
        SetupManager setup;

        int task_count;

        // Singleton
        System();

        void changeState(TCBState state, PreemptType type = PreemptType::NONE);
        void checkNewTasks();
        void terminateTask();
        void suspendTask();
        void preemptTask(PreemptType type);
    
        void endProgram();
        
    public:
        ~System();
        static System* getInstance();

        void loadConfig();
        void handleInterruption(Interruption irq);
        void startTick();
        void endTick();
    };
} // namespace Core