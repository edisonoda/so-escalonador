#include "system.hpp"

#include "clock/clock.hpp"

using namespace Core;

System *System::instance(nullptr);

System::System()
    : scheduler(Scheduler::Scheduler::getInstance()),
      clock(this),
      chart_generator(&ord_tasks),
      gantt_chart(&chart_generator),
      screen(Screen::getInstance())
{
    current_task = nullptr;
    task_count = 0;
    avg_turnaround = 0;
    avg_wait = 0;

    scheduler->setTaskList(&ready_list);
}

System::~System()
{
    delete screen;
    delete scheduler;

    instance = nullptr;
    screen = nullptr;
    scheduler = nullptr;
    current_task = nullptr;

    for (TCB *task : ord_tasks)
        delete task;

    ord_tasks.clear();
    new_list.clear();
    ready_list.clear();
    suspended_list.clear();
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

    if (current_task != nullptr && current_task->getRemaining() > 0)
        current_task->decrementRemaining(1);

    gantt_chart.drawTick(clock.getTotalTime());
    task_info.drawTick(clock.getTotalTime());

    if (current_task != nullptr && current_task->getRemaining() <= 0)
        terminateTask();
}

void System::handleInterruption(Interruption irq)
{
    switch (irq)
    {
    case Interruption::QUANTUM:
        preemptTask(Scheduler::PreemptType::QUANTUM);
        break;
    case Interruption::FULL_STOP:
        getch();
        break;
    default:
        break;
    }
}

void System::changeState(TCBState state, Scheduler::PreemptType type)
{
    TCB *previous_task = current_task;
    if (current_task != nullptr)
        current_task->setState(state);

    current_task = scheduler->chooseTask(current_task, type);

    if (current_task != nullptr)
    {
        switch (current_task->getState())
        {
        case TCBState::SUSPENDED:
            suspended_list.remove(current_task);
            break;
        case TCBState::READY:
            ready_list.remove(current_task);
            break;
        default:
            break;
        }

        if (previous_task != current_task)
            clock.resetQuantum();
    }

    if (current_task != nullptr)
        current_task->setState(TCBState::RUNNING);

    Log("T=" + to_string(clock.getTotalTime()));
    for (TCB* task:ready_list)
    {
        Log(" | " + task->getId());
    }
}

void System::checkNewTasks()
{
    bool new_task_arrived = false;
    list<TCB *>::iterator i = new_list.begin();

    while (i != new_list.end())
    {
        if ((*i)->getStart() <= clock.getTotalTime())
        {
            (*i)->setState(TCBState::READY);
            ready_list.push_back((*i));
            new_list.erase(i++);
            new_task_arrived = true;
        }
        else
        {
            i++;
        }
    }

    if (new_task_arrived)
        preemptTask(Scheduler::PreemptType::NEW_TASK);
}

void System::terminateTask()
{
    changeState(TCBState::TERMINATED);

    task_count--;
    if (task_count <= 0)
    {
        tick();
        endProgram();
    }
}

void System::suspendTask()
{
    suspended_list.push_back(current_task);
    changeState(TCBState::SUSPENDED);
}

void System::preemptTask(Scheduler::PreemptType type)
{
    clock.resetQuantum();

    if (current_task != nullptr)
    {
        if (current_task->getRemaining() <= 0)
        {
            terminateTask();
        }
        else
        {
            ready_list.push_back(current_task);
            changeState(TCBState::READY, type);
        }
    }
    else
        changeState(TCBState::READY, type);
}

void System::calcAverageTimes()
{
    double total_turnaround_time = 0;
    double total_wait_time = 0;
    int task_n = ord_tasks.size();

    for (TCB *task : ord_tasks)
    {
        int arrival = task->getStart();
        int duration = task->getDuration();
        int completion = task->getCompletionTime();

        int turnaround = completion - arrival;
        total_turnaround_time += turnaround;

        int wait = turnaround - duration;
        total_wait_time += wait;
    }

    avg_turnaround = (task_n > 0) ? (total_turnaround_time / task_n) : 0;
    avg_wait = (task_n > 0) ? (total_wait_time / task_n) : 0;
}

void System::loadConfig()
{
    SimulationConfig configs = setup.run();

    if (!configs.simulation_should_run)
        return;

    screen->erase();

    scheduler->setAlgorithm(configs.alg_id);
    clock.setQuantum(configs.quantum);

    ord_tasks = configs.tasks;
    new_list = list<TCB *>(begin(ord_tasks), end(ord_tasks));
    task_count = new_list.size();

    gantt_chart.setTasks(&ord_tasks);
    task_info.setTasks(&ord_tasks);
    task_info.positionCorrectionForSimulation(gantt_chart.getHeight());

    task_info.drawTick(0);
    clock.selectMode(configs.mode);
    clock.run();
}

void System::endProgram()
{
    clock.stop();
    chart_generator.generate("chart.svg", clock.getTotalTime(), ord_tasks.size());
    calcAverageTimes();
    task_info.displayFinalStatistics(avg_turnaround, avg_wait);
    timeout(-1);
    flushinp();
    handleInterruption(Interruption::FULL_STOP);
    screen->erase();
    screen->refresh();
}