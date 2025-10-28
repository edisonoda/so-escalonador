#pragma once

#include <list>
#include <vector>
#include "tcb.hpp"
#include "clock/clock.hpp"
#include "../scheduler/scheduler.hpp"
#include "../ui/screen.hpp"
#include "../ui/gantt_chart.hpp"
#include "../ui/system_monitor.hpp"
#include "../utils/chart_generator.hpp"
#include "setup_manager.hpp"

using namespace std;

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

        UI::Screen* screen;
        UI::GanttChart gantt_chart;
        UI::SystemMonitor system_monitor;

        Utils::ChartGenerator chart_generator;

        int task_count;

        SetupManager setup;

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

        bool loadConfig(const string &filename);
        void handleInterruption(Interruption irq);
        void tick();
    };
} // namespace Core