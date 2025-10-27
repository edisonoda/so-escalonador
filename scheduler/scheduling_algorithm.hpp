#pragma once

#include <list>
#include "../core/tcb.hpp"

using namespace std;
using namespace Core;

namespace Scheduler
{
    enum class AlgorithmID
    {
        FCFS,
        SRTF,
        PRIOp
    };

    class SchedulingAlgorithm
    {
    protected:
        const AlgorithmID id;
        list<TCB *> *task_list;

    public:
        SchedulingAlgorithm(AlgorithmID id, list<TCB *> *task_list) : id(id), task_list(task_list) {};
        virtual ~SchedulingAlgorithm() {};

        AlgorithmID getID() const { return id; }

        virtual TCB *chooseTask(TCB *current_task = nullptr) = 0;
    };
} // namespace Scheduler