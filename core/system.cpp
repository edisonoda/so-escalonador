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
            cout << "System Tick: " << clock.getTotalTime() << endl;
            checkNewTasks();
            tick_count++;
            cout << "Tick Count: " << tick_count << endl;
            if (tick_count >= quantum)
            {
                onQuantum();
                tick_count = 0;
            }
            else
            {
                tick();
            }
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
        {
            suspendCurrentTask(TCBState::TERMINATED);
        }
    }
    else
    {
        current_task = scheduler->chooseTask();
    }
}

void System::onQuantum()
{
    cout << "Quantum expired" << endl;

    if (current_task != nullptr)
    {
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
            break;

        default:
            cout << "Terminating task: " << current_task->getId() << endl;
            current_task->setState(TCBState::TERMINATED);
            break;
        }
        current_task = scheduler->chooseTask();
        if (current_task != previous_task)
        {
            tick_count = 0;
        }
    }
}

void System::checkNewTasks()
{
    // Fazer remoção depois do for
    for (TCB *task : loaded_list)
    {
        if (task->getStart() >= clock.getTotalTime())
        {
            cout << "Loading task: " << task->getId() << endl;
            task->setState(TCBState::READY);
            ready_list.push_back(task);
            loaded_list.remove(task);
            cout << "Task loaded: " << task->getId() << endl;
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