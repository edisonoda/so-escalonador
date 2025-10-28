#include "system.hpp"
#include "clock/clock.hpp"

using namespace Core;

System *System::instance(nullptr);

System::System() : TickObserver()
    , scheduler(Scheduler::Scheduler::getInstance())
    , clock(this)
    , chart_generator(&ord_tasks)
    , gantt_chart(&chart_generator)
{
    current_task = nullptr;
    task_count = 0;

    clock.attach(this);

    scheduler->setTaskList(&ready_list);
    screen = UI::Screen::getInstance();
}

System::~System()
{
    delete screen;
    delete scheduler;
    
    instance = nullptr;
    current_task = nullptr;
    screen = nullptr;
    scheduler = nullptr;

    for (TCB* task : ord_tasks)
        delete task;

    for (IOEvent* ev : event_list)
        delete ev;

    ord_tasks.clear();
    new_list.clear();
    ready_list.clear();
    suspended_list.clear();
    event_list.clear();

    clock.detach(this);
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

        checkEvents();

        if (current_task != nullptr)
            current_task->decrementRemaining(1);
    }

    gantt_chart.drawTick(clock.getTotalTime());
    system_monitor.drawTick(clock.getTotalTime());
}

void System::handleInterruption(Interruption irq, TCB* task)
{
    switch (irq)
    {
    case Interruption::QUANTUM:
        clock.resetQuantum();
        preemptTask();
        break;
    case Interruption::FINISH_IO:
        if (task != nullptr)
            readyTask(task);
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
        case TCBState::SUSPENDED:
            suspended_list.remove(current_task);
            break;

        case TCBState::READY:
            ready_list.remove(current_task);
            break;
        default:
            break;
        }
        
        current_task->setState(TCBState::RUNNING);

        if (previous_task != current_task)
            clock.resetQuantum();
    }
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

void System::checkEvents()
{
    list<Event*>* events = current_task->getEvents();

    if (!events || events->empty())
        return;

    list<Event*>::iterator i = (*events).begin();
    int elapsed = current_task->getDuration() - current_task->getRemaining();

    while (i != (*events).end())
    {
        if ((*i)->start <= elapsed)
        {
            event_list.push_back(new IOEvent(current_task, this, &clock, (*i)->duration));
            events->erase(i++);
            delete (*i);
            suspendTask();
            return;
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
    if (current_task != nullptr)
        ready_list.push_back(current_task);

    changeState(TCBState::READY);
}

void System::readyTask(TCB* task)
{
    suspended_list.remove(task);
    ready_list.push_back(task);
    task->setState(TCBState::READY);
    preemptTask();
}

bool System::loadConfig(const string &filename)
{
    char mode = clock.initialSelection();

    if (!config_reader.openFile(filename))
    {
        printf("Failed to open config file.\n");
        return false;
    }

    config_reader.readPattern();
    scheduler->setAlgorithm(config_reader.getAlgorithm());
    clock.setQuantum(config_reader.getQuantum());

    new_list = config_reader.readTasks();
    task_count = new_list.size();

    for (TCB *task : new_list)
        screen->initColor(0, task->getColor());

    ord_tasks = vector<TCB *>(begin(new_list), end(new_list));

    gantt_chart.setScreen(screen);
    gantt_chart.setTasks(&ord_tasks);

    system_monitor.setOffset(task_count + 2);
    system_monitor.setScreen(screen);
    system_monitor.setTasks(&ord_tasks);
    system_monitor.drawTick(0);
    
    clock.selectMode(mode);
    clock.run();

    return true;
}