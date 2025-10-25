#pragma once

#include <list>
#include "../core/tcb.hpp"
#include "scheduling_algorithm.hpp"

using namespace std;
using namespace Core;

namespace Scheduler
{
    class Scheduler
    {
    private:
        static Scheduler *instance;

        list<TCB *> *task_list;
        SchedulingAlgorithm *algorithm;

        // Singleton
        Scheduler();

    public:
        ~Scheduler();
        static Scheduler *getInstance();

        void setAlgorithm(AlgorithmID id);
        void setTaskList(list<TCB *> *task_list);
        TCB *chooseTask(TCB *current_task = nullptr);
    };
} // namespace Scheduler