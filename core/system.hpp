#pragma once

#include <list>
#include "tcb.hpp"
#include "config_reader.hpp"
#include "system_clock.hpp"
#include "../scheduler/scheduler.hpp"

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
        list<TCB*> new_list;
        list<TCB*> ready_list;
        list<TCB*> suspended_list;

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