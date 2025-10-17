#pragma once

#include <list>
#include "../core/tcb.hpp"
#include "scheduling_algorithm.hpp"

using namespace std;

namespace Scheduler
{
    class Scheduler
    {
    private:
        static Scheduler* instance;

        list<Core::TCB*>* task_list;
        SchedulingAlgorithm* algorithm;

        // Singleton
        Scheduler();
    
    public:
        ~Scheduler();
        static Scheduler* getInstance();

        // void setAlgorithm(AlgorithmID id);
        void setAlgorithm(string id);
        void setTaskList(list<Core::TCB*>* task_list);
        Core::TCB* chooseTask();
    };
} // namespace Scheduler