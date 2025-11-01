#include "fcfs_algorithm.hpp"

using namespace Scheduler;

FCFS::FCFS(list<TCB *> *task_list) : SchedulingAlgorithm(AlgorithmID::FCFS, task_list)
{
}

FCFS::~FCFS()
{
}
    
TCB *FCFS::chooseTask(TCB *current_task, PreemptType type)
{
    if (task_list->empty())
        return nullptr;
 
    if (type == PreemptType::NEW_TASK)
        return current_task;
    
    return task_list->front();
}