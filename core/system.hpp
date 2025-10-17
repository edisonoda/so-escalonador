#pragma once

#include <list>
#include "tcb.hpp"
#include "config_reader.hpp"
#include "../scheduler/scheduler.hpp"

using namespace std;

namespace Core
{
    class System
    {
    private:
        static System* instance;
        static float frequency;
        
        Scheduler::Scheduler* scheduler;
        ConfigReader configReader;
        list<TCB*> task_list;
        list<TCB*> ready_list;
        list<TCB*> suspended_list;

        int quantum;

        // Singleton
        System();

        void tick();
    
    public:
        ~System();
        static System* getInstance();

        bool loadConfig(const string &filename);
        void run();
    };
} // namespace Core