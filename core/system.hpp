#pragma once

#include <list>
#include <vector>
#include "tcb.hpp"
#include "config_reader.hpp"
#include "system_clock.hpp"
#include "../scheduler/scheduler.hpp"
#include "../ui/screen.hpp"
#include "../ui/gantt_chart.hpp"
#include "../ui/system_monitor.hpp"

using namespace std;

namespace Core
{
    class System
    {
    private:
        static System* instance;
        
        Scheduler::Scheduler* scheduler;
        SystemClock clock;
        ConfigReader config_reader;
        TCB* current_task;
        vector<TCB*> ord_tasks;
        list<TCB*> new_list;
        list<TCB*> ready_list;
        list<TCB*> suspended_list;

        UI::Screen* screen;
        UI::GanttChart* gantt_chart;
        UI::SystemMonitor* system_monitor;

        bool running;
        int quantum;
        int tick_count;
        int task_count;

        // Singleton
        System();

        void run();
        void tick();
        void suspendCurrentTask(TCBState state);
        void checkNewTasks();
    
    public:
        ~System();
        static System* getInstance();

        bool loadConfig(const string &filename);
    };
} // namespace Core