#include "system.hpp"
#include "clock/auto_clock.hpp"
#include "clock/manual_clock.hpp"

using namespace Core;

System *System::instance(nullptr);

System::System()
    : scheduler(Scheduler::Scheduler::getInstance())
{
    current_task = nullptr;
    task_count = 0;

    scheduler->setTaskList(&ready_list);
    clock = nullptr;
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
        changeState(TCBState::RUNNING);
        clock->resetQuantum();
    }

    if (current_task != nullptr)
    {
        if (current_task->getRemaining() <= 0)
            terminateTask();

        if (current_task != nullptr)
            current_task->decrementRemaining(1);
    }

    gantt_chart.drawTick(clock->getTotalTime());
    system_monitor.drawTick(clock->getTotalTime());
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
            clock->resetQuantum();
    }

    if (current_task != nullptr)
        current_task->setState(TCBState::RUNNING);
}

void System::checkNewTasks()
{
    list<TCB *>::iterator i = new_list.begin();

    while (i != new_list.end())
    {
        if ((*i)->getStart() <= clock->getTotalTime())
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

void System::selectClock(char mode)
{
    int total_time = 0;

    if (clock != nullptr)
    {
        total_time = clock->getTotalTime();
        delete clock;
    }

    switch (mode)
    {
    case 'A':
        clock = new AutoClock(this, total_time);
        break;
    case 'P':
        clock = new ManualClock(this, total_time);
        break;
    default:
        clock = new AutoClock(this, total_time);
        break;
    }
}

void System::terminateTask()
{
    task_count--;

    if (task_count <= 0)
        clock->stop();

    changeState(TCBState::TERMINATED);
}

void System::suspendTask()
{
    suspended_list.push_back(current_task);
    changeState(TCBState::SUSPENDED);
}

void System::preemptTask()
{
    clock->resetQuantum();

    if (current_task != nullptr)
        ready_list.push_back(current_task);

    changeState(TCBState::READY);
}

bool System::loadConfig(const string &filename)
{
    char mode = '\0';

    screen->print(0, 0, "Selecione um modo de execução:");
    screen->print(0, 1, "- Automático = digite 'A'");
    screen->print(0, 2, "- Passo a passo = digite 'P'");
    screen->print(0, 3, "Escolha: ");
    screen->refresh();

    while (mode != 'A' && mode != 'P')
        mode = toupper(screen->getCh());

    selectClock(mode);
    screen->clear();

    if (!config_reader.openFile(filename))
    {
        printf("Failed to open config file.\n");
        return false;
    }

    config_reader.readPattern();
    clock->setQuantum(config_reader.getQuantum());
    scheduler->setAlgorithm(config_reader.getAlgorithm());

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
    
    clock->run();

    return true;
}