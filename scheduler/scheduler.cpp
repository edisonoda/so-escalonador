#include <iostream>
#include "scheduler.hpp"
using namespace std;

namespace Scheduler
{
    Scheduler* Scheduler::instance(nullptr);

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

    Core::TCB* Scheduler::chooseTask()
    {
        // temporário
        Core::TCB* task = task_list->front();
        task_list->pop_front();
        cout << "Chosen task: " << task->getId() << endl;
        return task;
    }
}
