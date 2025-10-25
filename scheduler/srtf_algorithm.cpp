#include "srtf_algorithm.hpp"

using namespace Scheduler;

SRTF::SRTF(list<TCB *> *task_list) : SchedulingAlgorithm(task_list)
{
}

SRTF::~SRTF()
{
}

TCB *SRTF::chooseTask(TCB *current_task)
{
    if (task_list->empty())
    {
        if (current_task == nullptr || current_task->getState() == TCBState::TERMINATED)
            return nullptr;
        
        return current_task;
    }

    if (current_task == nullptr || current_task->getState() != TCBState::READY)
        current_task = task_list->front();
    
    for (TCB *task : *task_list)
    {
        if (task->getRemaining() < current_task->getRemaining())
            current_task = task;
    }

    return current_task;
}