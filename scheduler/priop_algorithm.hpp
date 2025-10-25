#pragma once

#include "scheduling_algorithm.hpp"

namespace Scheduler
{
    class PRIOp : public SchedulingAlgorithm
    {
        public:
            PRIOp(list<TCB*>* task_list);
            ~PRIOp();
            virtual TCB* chooseTask(TCB* current_task = nullptr);
    };
} // namespace Scheduler