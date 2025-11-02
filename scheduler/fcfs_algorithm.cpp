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
    // Se a lista não tiver mais tasks para executar retorna nulo
    if (task_list->empty())
        return nullptr;
 
    // Se ainda houver tasks e a preempção foi por tempo, retorna a task atual
    if (type == PreemptType::NEW_TASK)
        return current_task;
    
    // Se ainda tem tasks e a preempção não foi por tempo, retorna a que entrou primeiro
    return task_list->front();
}