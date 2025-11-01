#pragma once

#include "scheduling_algorithm.hpp"

namespace Scheduler
{
    class SRTF : public SchedulingAlgorithm
    {
        public:
            SRTF(list<TCB*>* task_list);
            ~SRTF();
            virtual TCB* chooseTask(TCB* current_task = nullptr, PreemptType type = PreemptType::NONE);
    };
} // namespace Scheduler