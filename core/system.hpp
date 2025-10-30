#pragma once

#include "../utils/std_libraries.hpp"

#include "tcb.hpp"
#include "clock/clock.hpp"
#include "setup_manager.hpp"

#include "../scheduler/scheduler.hpp"

#include "../ui/screen.hpp"
#include "../ui/gantt_chart.hpp"
#include "../ui/system_monitor.hpp"

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
        // ConfigReader config_reader;
        TCB* current_task;
        vector<TCB*> ord_tasks;
        list<TCB*> new_list;
        list<TCB*> ready_list;
        list<TCB*> suspended_list;

        Screen* screen;
        GanttChart gantt_chart;
        SystemMonitor system_monitor;

        Utils::ChartGenerator chart_generator;
        SetupManager setup;

        int task_count;

        // Singleton
        System();

        void changeState(TCBState state);
        void checkNewTasks();
        void terminateTask();
        void suspendTask();
        void preemptTask();
    
    public:
        ~System();
        static System* getInstance();

        bool loadConfig();
        void handleInterruption(Interruption irq);
        void tick();
    };
} // namespace Core