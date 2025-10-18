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
    if (current_task != nullptr)
    {
        current_task->decrementRemaining(1);

        if (current_task->getRemaining() <= 0)
            suspendCurrentTask(TCBState::TERMINATED);
        else if (tick_count >= quantum)
            suspendCurrentTask(TCBState::READY);
    }
    else
    {
        current_task = scheduler->chooseTask();
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
            current_task->setState(TCBState::SUSPENDED);
            suspended_list.push_back(current_task);
            break;

        case TCBState::READY:
            cout << "Re-queuing task: " << current_task->getId() << endl;
            current_task->setState(TCBState::READY);
            ready_list.push_back(current_task);
            tick_count = 0;
            break;

        default:
            cout << "Terminating task: " << current_task->getId() << endl;
            current_task->setState(TCBState::TERMINATED);
            task_count--;
            
            if (task_count <= 0)
                running = false;

            break;
        }

        current_task = scheduler->chooseTask();
        if (current_task != previous_task)
            tick_count = 0;
    }
}

void System::checkNewTasks()
{
    list<TCB*>::iterator i = loaded_list.begin();

    while (i != loaded_list.end())
    {
        if ((*i)->getStart() <= clock.getTotalTime())
        {
            cout << "Loading task: " << (*i)->getId() << endl;
            (*i)->setState(TCBState::READY);
            ready_list.push_back((*i));
            loaded_list.erase(i++);
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

    loaded_list = config_reader.readTasks();
    task_count = loaded_list.size();

    for (TCB *task : loaded_list)
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