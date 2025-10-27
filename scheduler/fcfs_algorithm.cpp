#include "fcfs_algorithm.hpp"

using namespace Scheduler;

FCFS::FCFS(list<TCB *> *task_list) : SchedulingAlgorithm(AlgorithmID::FCFS, task_list)
{
}

FCFS::~FCFS()
{
}

TCB *FCFS::chooseTask(TCB *current_task)
{
    if (current_task != nullptr && current_task->getState() == TCBState::READY)
    {
        return current_task;
    }

    if (task_list->empty())
        return nullptr;
        
    return task_list->front();
}