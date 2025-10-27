#include "priop_algorithm.hpp"

using namespace Scheduler;

PRIOp::PRIOp(list<TCB *> *task_list) : SchedulingAlgorithm(AlgorithmID::PRIOp, task_list)
{
}

PRIOp::~PRIOp()
{
}

TCB *PRIOp::chooseTask(TCB *current_task)
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
        if (task->getPriority() > current_task->getPriority())
            current_task = task;
    }

    return current_task;
}