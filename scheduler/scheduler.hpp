#pragma once

#include "../utils/std_libraries.hpp"

#include "scheduling_algorithm.hpp"

#include "../core/tcb.hpp"

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
        TCB *chooseTask(TCB *current_task = nullptr, PreemptType type = PreemptType::NONE);
    };
} // namespace Scheduler