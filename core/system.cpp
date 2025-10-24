#include "system.hpp"

using namespace Core;

System *System::instance(nullptr);

System::System()
    : scheduler(Scheduler::Scheduler::getInstance())
    , clock(this)
{
    current_task = nullptr;
    task_count = 0;

    scheduler->setTaskList(&ready_list);
    screen = UI::Screen::getInstance();
}

System::~System()
{
    instance = nullptr;
    delete screen;
    delete scheduler;
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
        current_task = scheduler->chooseTask();
        clock.resetQuantum();
    }

    if (current_task != nullptr)
    {
        if (current_task->getRemaining() <= 0)
            suspendCurrentTask(TCBState::TERMINATED);

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
        suspendCurrentTask(TCBState::READY);
        break;
    case Interruption::FULL_STOP:
        screen->getCh();
        break;
    }
}

void System::suspendCurrentTask(TCBState state)
{
    if (current_task != nullptr)
    {
        TCB *previous_task = current_task;
        current_task->setState(state);

        switch (state)
        {
        case TCBState::SUSPENDED:
            suspended_list.push_back(current_task);
            break;

        case TCBState::READY:
            clock.resetQuantum();
            break;

        default:
            task_count--;
            
            if (task_count <= 0)
                clock.stop();

            break;
        }

        current_task = scheduler->chooseTask(current_task);
        if (current_task != previous_task)
            clock.resetQuantum();
    }
}

void System::checkNewTasks()
{
    list<TCB*>::iterator i = new_list.begin();

    while (i != new_list.end())
    {
        if ((*i)->getStart() <= clock.getTotalTime())
        {
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
    clock.setQuantum(config_reader.getQuantum());
    scheduler->setAlgorithm(config_reader.getAlgorithm().c_str());

    new_list = config_reader.readTasks();
    task_count = new_list.size();

    for (TCB *task : new_list)
        screen->initColor(0, task->getColor());

    ord_tasks = vector<TCB*>(begin(new_list), end(new_list));

    gantt_chart.setScreen(screen);
    gantt_chart.setTasks(&ord_tasks);

    system_monitor.setOffset(task_count + 2);
    system_monitor.setScreen(screen);
    system_monitor.setTasks(&ord_tasks);
    system_monitor.drawTick(0);

    clock.run();

    return true;
}