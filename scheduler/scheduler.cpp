#include "scheduler.hpp"
using namespace std;

namespace Scheduler
{
    Scheduler* Scheduler::instance(nullptr);

    Scheduler::Scheduler() : task_list(nullptr), algorithm(nullptr) { }

    Scheduler::~Scheduler() 
    {
        instance = nullptr;
        delete algorithm;
    }

    Scheduler* Scheduler::getInstance()
    {
        if (instance == nullptr)
            instance = new Scheduler();
            
        return instance;
    }

    void Scheduler::setAlgorithm(string id)
    {
        algorithm = new FCFS(task_list);
    }

    void Scheduler::setTaskList(list<TCB*>* task_list)
    {
        this->task_list = task_list;
    }

    TCB* Scheduler::chooseTask(TCB* current_task)
    {
        return algorithm->chooseTask(current_task);
    }
}
