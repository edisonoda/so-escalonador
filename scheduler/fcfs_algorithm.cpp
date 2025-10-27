#include "fcfs_algorithm.hpp"

using namespace Scheduler;

FCFS::FCFS(list<TCB *> *task_list) : SchedulingAlgorithm(task_list)
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

    TCB *task = task_list->front();
    // task_list->pop_front(); // Os outros algoritmos não estão operando as listas, só 
    // encontrando a nova task, então não precisa desse pop front, pois la na system ela já 
    // remove    
    return task;
}