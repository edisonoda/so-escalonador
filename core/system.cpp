#include "system.hpp"

#include "../utils/constants.hpp"

#include <iostream>

using namespace Core;

System *System::instance(nullptr);

System::System() : scheduler(Scheduler::Scheduler::getInstance())
{
    current_task = nullptr;
    scheduler->setTaskList(&ready_list);
    quantum = Constants::DEFAULT_QUANTUM;
    tick_count = 0;
    task_count = 0;
    running = true;
}

System::~System()
{
    delete scheduler;
}

System *System::getInstance()
{
    if (instance != nullptr)
        return instance;

    instance = new System();
    return instance;
}

void System::run()
{
    while (running)
    {
        if (clock.getTick())
        {
            cout << "============================================" << endl;
            cout << "System Tick: " << clock.getTotalTime() << endl;
            checkNewTasks();
            tick_count++;
            cout << "Tick Count: " << tick_count << endl;
            tick();
        }
    }
}

void System::tick()
{
    cout << "Ticked" << endl;

    if (current_task == nullptr) {
        current_task = scheduler->chooseTask();
        tick_count = 0;
    }

    if (current_task != nullptr)
    {
        current_task->decrementRemaining(1);
        cout << "Running task: " << current_task->getId()
             << ", Remaining time: " << current_task->getRemaining() << endl;

        if (current_task->getRemaining() <= 0)
            suspendCurrentTask(TCBState::TERMINATED);
        else if (tick_count >= quantum)
            suspendCurrentTask(TCBState::READY);
    }
}

void System::suspendCurrentTask(TCBState state)
{
    if (current_task != nullptr)
    {
        TCB *previous_task = current_task;

        switch (state)
        {
        case TCBState::SUSPENDED:
            cout << "Suspending task: " << current_task->getId() << endl;
            current_task->setState(state);
            suspended_list.push_back(current_task);
            break;

        case TCBState::READY:
            cout << "Re-queuing task: " << current_task->getId() << endl;
            // current_task->setState(TCBState::READY);
            // ready_list.push_back(current_task);
            tick_count = 0;
            break;

        default:
            cout << "Terminating task: " << current_task->getId() << endl;
            current_task->setState(state);
            task_count--;
            cout << "Remaining tasks: " << task_count << endl;
            
            if (task_count <= 0)
                running = false;

            break;
        }

        current_task = scheduler->chooseTask(current_task);
        if (current_task != previous_task)
            tick_count = 0;
    }
}

void System::checkNewTasks()
{
    list<TCB*>::iterator i = new_list.begin();

    while (i != new_list.end())
    {
        if ((*i)->getStart() <= clock.getTotalTime())
        {
            cout << "Loading task: " << (*i)->getId() << endl;
            (*i)->setState(TCBState::READY);
            ready_list.push_back((*i));
            new_list.erase(i++);
        }
        else
        {
            i++;
        }
    }
}

bool System::loadConfig(const string &filename)
{
    if (!config_reader.openFile(filename))
    {
        printf("Failed to open config file.\n");
        return false;
    }

    config_reader.readPattern();
    quantum = config_reader.getQuantum();
    scheduler->setAlgorithm(config_reader.getAlgorithm().c_str());

    new_list = config_reader.readTasks();
    task_count = new_list.size();

    for (TCB *task : new_list)
    {
        cout << "Loaded Task: " << task->getId()
             << ", Start: " << task->getStart()
             << ", Duration: " << task->getDuration()
             << ", Priority: " << task->getPriority() << endl;
    }

    cout << "Starting system with quantum: " << quantum << endl;

    run();

    return true;
}