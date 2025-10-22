#include <iostream>
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
        
    }

    void Scheduler::setTaskList(list<TCB*>* task_list)
    {
        this->task_list = task_list;
    }

    TCB* Scheduler::chooseTask(TCB* current_task)
    {
        // temporário
        if (current_task != nullptr && current_task->getState() == TCBState::READY)
        {
            current_task->setState(TCBState::RUNNING);
            return current_task;
        }

        if (task_list->empty())
            return nullptr;

        TCB* task = task_list->front();
        task_list->pop_front();
        // cout << "Chosen task: " << task->getId() << endl;
        task->setState(TCBState::RUNNING);
        return task;
    }
}
