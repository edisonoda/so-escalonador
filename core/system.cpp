#include "system.hpp"

#include "clock.hpp"

using namespace Core;

System *System::instance(nullptr);

System::System()
    : scheduler(Scheduler::getInstance()),
      clock(this),
      chart_generator(&ord_tasks),
      gantt_chart(&chart_generator),
      screen(Screen::getInstance())
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

void System::startTick()
{
    checkNewTasks();

    // Se não existe task em execução, busca uma task
    if (current_task == nullptr)
        changeState(TCBState::RUNNING);

    // Se existe task em execução, mas o tempo restante de execução é 0, termina a task
    if (current_task != nullptr && current_task->getRemaining() <= 0)
        terminateTask();
}

void System::endTick()
{
    // Se existe task em execução decrementa o tempo restante de execução da mesma
    if (current_task != nullptr)
        current_task->decrementRemaining(1);

    // Atualiza o gráfico e as informações das tarefas
    gantt_chart.drawTick(clock.getTotalTime());
    task_info.drawTick(clock.getTotalTime());
}

void System::handleInterruption(Interruption irq)
{
    switch (irq)
    {
    case Interruption::QUANTUM:
        preemptTask(PreemptType::QUANTUM);
        break;
    case Interruption::FULL_STOP:
        getch();
        break;
    default:
        break;
    }
}

void System::changeState(TCBState state, PreemptType type)
{
    TCB *previous_task = current_task;

    // Se existe task em execução, faz a troca de estado
    if (current_task != nullptr)
        current_task->setState(state);

    // Chama o escalonador
    current_task = scheduler->chooseTask(current_task, type);

    // Se o escalonador escolheu uma task, remove ela das listas
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

        // Se houve troca de tarefa, reinicia o quantum
        if (previous_task != current_task)
            clock.resetQuantum();
        
        current_task->setState(TCBState::RUNNING);
    }
}

void System::checkNewTasks()
{
    bool new_task_arrived = false;
    list<TCB *>::iterator i = new_list.begin();

    while (i != new_list.end())
    {
        // Se o tick atual for igual ao inicio da task, adiciona ela à lista de prontas
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

    // Se houve ingresso de nova task, faz a preempção
    if (new_task_arrived)
        preemptTask(PreemptType::NEW_TASK);
}

void System::terminateTask()
{
    // Armazena o tempo em que a task foi terminada para o cálculo dos tempos médios
    current_task->setCompletionTime(clock.getTotalTime());

    changeState(TCBState::TERMINATED);

    task_count--;

    // Verifica se ainda existem tasks para execução
    if (task_count == 0)
        endProgram();
}

void System::suspendTask()
{
    suspended_list.push_back(current_task);
    changeState(TCBState::SUSPENDED);
}

void System::preemptTask(PreemptType type)
{
    // Se a task em execução ainda não tiver terminado, coloca ela de volta na lista de prontas
    if (current_task != nullptr && current_task->getRemaining() > 0)
        ready_list.push_back(current_task);

    changeState(TCBState::READY, type);
}

void System::loadConfig()
{
    // Executa o menu de configurações
    SimulationConfig configs = setup.run();

    // Verifica se o usuário seleciou a saída do programa
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
    task_info.moveWindow(0, gantt_chart.getHeight());
    task_info.drawTick(0);
    
    clock.selectMode(configs.mode);
    clock.run();
}

void System::endProgram()
{
    startTick();
    endTick();
    
    clock.stop();
    chart_generator.generate("chart.svg", clock.getTotalTime(), ord_tasks.size());
    task_info.displayFinalStatistics();

    // Retorna o tipo de input para bloqueante e limpa o buffer
    timeout(-1);
    flushinp();
    handleInterruption(Interruption::FULL_STOP);
    screen->erase();
    screen->refresh();
}