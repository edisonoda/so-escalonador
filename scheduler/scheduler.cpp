#include "scheduler.hpp"

namespace Scheduler
{
    Scheduler* Scheduler::instance(NULL);

    Scheduler::Scheduler() : task_list(nullptr), algorithm(nullptr) { }

    Scheduler::~Scheduler() { }

    Scheduler* Scheduler::getInstance()
    {
        if (instance != nullptr)
            return instance;

        instance = new Scheduler();
        return instance;
    }

    void Scheduler::setAlgorithm(string id)
    {
        
    }

    void Scheduler::setTaskList(list<Core::TCB*>* task_list)
    {
        
    }

    void Scheduler::chooseTask()
    {

    }
}
