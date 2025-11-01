#pragma once

#include "../utils/std_libraries.hpp"

#include "tcb.hpp"
#include "clock/clock.hpp"
#include "setup_manager.hpp"

#include "../scheduler/scheduler.hpp"
#include "../scheduler/scheduling_algorithm.hpp"

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
        
        Scheduler::Scheduler* scheduler;
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
        double avg_turnaround;
        double avg_wait;

        // Singleton
        System();

        void changeState(TCBState state, Scheduler::PreemptType type = Scheduler::PreemptType::NONE);
        void checkNewTasks();
        void terminateTask();
        void suspendTask();
        void preemptTask(Scheduler::PreemptType type);
        void calcAverageTimes();
    
        void endProgram();
        
    public:
        ~System();
        static System* getInstance();

        void loadConfig();
        void handleInterruption(Interruption irq);
        void tick();
    };
} // namespace Core