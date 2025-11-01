#pragma once

#include "scheduling_algorithm.hpp"

namespace Scheduler
{
    class FCFS : public SchedulingAlgorithm
    {
        public:
            FCFS(list<TCB*>* task_list);
            ~FCFS();
            virtual TCB* chooseTask(TCB* current_task = nullptr, PreemptType type = PreemptType::NONE);
    };
} // namespace Scheduler