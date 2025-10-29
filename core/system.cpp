#include "system.hpp"
#include "clock/clock.hpp"

using namespace Core;

System *System::instance(nullptr);

System::System()
    : scheduler(Scheduler::Scheduler::getInstance())
    , clock(this)
    , chart_generator(&ord_tasks)
    , gantt_chart(&chart_generator)
    , screen(UI::Screen::getInstance())
{
    current_task = nullptr;
    task_count = 0;

    scheduler->setTaskList(&ready_list);
}

System::~System()
{
    delete screen;
    delete scheduler;
    
    instance = nullptr;
    screen = nullptr;
    scheduler = nullptr;

    // TODO: delete all tasks
}

System *System::getInstance()
{
    if (instance == nullptr)
        instance = new System();

    return instance;
}

void System::tick()
{
    checkNewTasks();

    if (current_task == nullptr)
    {
        changeState(TCBState::RUNNING);
        clock.resetQuantum();
    }

    if (current_task != nullptr)
    {
        if (current_task->getRemaining() <= 0)
            terminateTask();

        if (current_task != nullptr)
            current_task->decrementRemaining(1);
    }

    gantt_chart.drawTick(clock.getTotalTime());
    system_monitor.drawTick(clock.getTotalTime());
}

void System::handleInterruption(Interruption irq)
{
    switch (irq)
    {
        case Interruption::QUANTUM:
            preemptTask();
            break;
        case Interruption::FULL_STOP:
            screen->getCh();
            break;
        default:
            break;
    }
}

void System::changeState(TCBState state)
{
    TCB *previous_task = current_task;
    if (current_task != nullptr)
        current_task->setState(state);

    current_task = scheduler->chooseTask(current_task);

    if (current_task != nullptr)
    {
        switch (current_task->getState())
        {
            case TCBState::SUSPENDED: suspended_list.remove(current_task); break;
            case TCBState::READY: ready_list.remove(current_task); break;
            default: break;
        }

        if (previous_task != current_task)
            clock.resetQuantum();
    }

    if (current_task != nullptr)
        current_task->setState(TCBState::RUNNING);
}

void System::checkNewTasks()
{
    list<TCB *>::iterator i = new_list.begin();

    while (i != new_list.end())
    {
        if ((*i)->getStart() <= clock.getTotalTime())
        {
            (*i)->setState(TCBState::READY);
            ready_list.push_back((*i));
            new_list.erase(i++);
            preemptTask();
        }
        else
        {
            i++;
        }
    }
}

void System::terminateTask()
{
    changeState(TCBState::TERMINATED);
    
    task_count--;

    if (task_count <= 0)
    {
        clock.stop();
        chart_generator.generate("chart.svg", clock.getTotalTime(), ord_tasks.size());
    }
}

void System::suspendTask()
{
    suspended_list.push_back(current_task);
    changeState(TCBState::SUSPENDED);
}

void System::preemptTask()
{
    clock.resetQuantum();

    if (current_task != nullptr)
        ready_list.push_back(current_task);

    changeState(TCBState::READY);
}

bool System::loadConfig()
{
    SimulationConfig configs = setup.run();
    
    screen->clear();

    scheduler->setAlgorithm(configs.alg_id);
    clock.setQuantum(configs.quantum);

    ord_tasks = configs.tasks;
    new_list = list<TCB *>(begin(ord_tasks), end(ord_tasks));
    task_count = new_list.size();

    gantt_chart.setScreen(screen);
    gantt_chart.setTasks(&ord_tasks);

    system_monitor.setOffset(task_count + 2);
    system_monitor.setScreen(screen);
    system_monitor.setTasks(&ord_tasks);
    system_monitor.drawTick(0);
    
    clock.selectMode(configs.mode);
    clock.run();

    return true;
}