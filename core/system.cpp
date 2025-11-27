#include "system.hpp"

#include "clock.hpp"

using namespace Core;

System *System::instance(nullptr);

IOEvent::IOEvent(TCB* task, System* sys, Clock* clock, const int d) : 
  TickObserver(),
  task(task),
  system(sys),
  clock(clock),
  duration(d)
{
  remaining_time = duration;
  clock->attach(this);
}

IOEvent::~IOEvent() {
  clock->detach(this);
  task = nullptr;
  system = nullptr;
  clock = nullptr;
}

void IOEvent::tick() {
  if (remaining_time <= 0)
    system->handleInterruption(Interruption::FINISH_IO, task);
  
  remaining_time--;
}

System::System() : 
  TickObserver(),
  scheduler(Scheduler::getInstance()),
  clock(this),
  gantt_exporter(&ord_tasks),
  gantt_chart(&gantt_exporter),
  screen(Screen::getInstance()) 
{
  current_task = nullptr;
  task_count = 0;

  clock.attach(this);

  scheduler->setTaskList(&ready_list);
  screen = UI::Screen::getInstance();
}

System::~System() {
  delete screen;
  delete scheduler;

  instance = nullptr;
  screen = nullptr;
  scheduler = nullptr;
  current_task = nullptr;

  for (TCB *task : ord_tasks)
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

System *System::getInstance() {
  if (instance == nullptr)
    instance = new System();

  return instance;
}

void System::tick() {
  checkNewTasks();

  // Se não existe task em execução, busca uma task
  if (current_task == nullptr)
    changeState(TCBState::RUNNING);
    
  // Se existe task em execução, mas o tempo restante de execução é 0, termina a task
  if (current_task != nullptr && current_task->getRemaining() <= 0)
    terminateTask();
  
  if (current_task != nullptr)
    checkEvents();
}

void System::endTick() {
  // Se existe task em execução decrementa o tempo restante de execução da mesma
  if (current_task != nullptr)
    current_task->decrementRemaining(1);

  // Atualiza o gráfico e as informações das tarefas
  gantt_chart.drawTick(clock.getTotalTime());
  task_info.drawTick(clock.getTotalTime());
}

void System::handleInterruption(Interruption irq, TCB* task) {
  switch (irq) {
    case Interruption::QUANTUM:
      preemptTask(PreemptType::QUANTUM);
      break;
    case Interruption::FINISH_IO:
      if (task != nullptr)
        readyTask(task);
      break;
    case Interruption::FULL_STOP:
      getch();
      break;
    default:
      break;
  }
}

void System::changeState(TCBState state, PreemptType type) {
  TCB *previous_task = current_task;

  // Se existe task em execução, faz a troca de estado
  if (current_task != nullptr)
    current_task->setState(state);

  // Chama o escalonador
  current_task = scheduler->chooseTask(current_task, type);

  // Se o escalonador escolheu uma task, remove ela das listas
  if (current_task != nullptr) {
    if (current_task->getState() == TCBState::READY)
      ready_list.remove(current_task);

    // Se houve troca de tarefa, reinicia o quantum
    if (previous_task != current_task)
      clock.resetQuantum();
    
    current_task->setState(TCBState::RUNNING);
  }
}

void System::checkNewTasks() {
  bool new_task_arrived = false;
  list<TCB *>::iterator i = new_list.begin();

  while (i != new_list.end()) {
    // Se o tick atual for igual ao inicio da task, adiciona ela à lista de prontas
    if ((*i)->getStart() <= clock.getTotalTime()) {
      (*i)->setState(TCBState::READY);
      ready_list.push_back((*i));
      new_list.erase(i++);
      new_task_arrived = true;
    } else {
      i++;
    }
  }

  // Se houve ingresso de nova task, faz a preempção
  if (new_task_arrived)
    preemptTask(PreemptType::NEW_TASK);
}

void System::checkEvents() {
  list<Event*>* events = current_task->getEvents();

  if (!events || events->empty())
    return;

  list<Event*>::iterator i = (*events).begin();
  int elapsed = current_task->getDuration() - current_task->getRemaining();

  while (i != (*events).end()) {
    if ((*i)->start <= elapsed) {
      IOEvent* event = new IOEvent(current_task, this, &clock, (*i)->duration);
      event_list.push_back(event);
      current_task->setCurrentEvent(event);
      delete (*i);
      i = events->erase(i);
      suspendTask();
      return;
    } else {
      i++;
    }
  }
}

void System::terminateTask() {
  // Armazena o tempo em que a task foi terminada para o cálculo dos tempos médios
  current_task->setCompletionTime(clock.getTotalTime());

  changeState(TCBState::TERMINATED);

  task_count--;

  // Verifica se ainda existem tasks para execução
  if (task_count == 0)
    endProgram();
}

void System::suspendTask() {
  if (current_task != nullptr && current_task->getRemaining() > 0)
    suspended_list.push_back(current_task); 

  changeState(TCBState::SUSPENDED);
}

void System::preemptTask(PreemptType type) {
  // Se a task em execução ainda não tiver terminado, coloca ela de volta na lista de prontas
  if (current_task != nullptr && current_task->getRemaining() > 0)
    ready_list.push_back(current_task);

  changeState(TCBState::READY, type);
}

void System::readyTask(TCB* task) {
  suspended_list.remove(task);
  ready_list.push_back(task);
  event_list.remove(task->getCurrentEvent());
  clock.scheduleDeletion(task->getCurrentEvent());
  task->setCurrentEvent(nullptr);
  task->setState(TCBState::READY);
  preemptTask(PreemptType::NEW_TASK);
}

void System::loadConfig() {
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

void System::endProgram() {
  tick();
  endTick();
  
  clock.stop();
  gantt_exporter.generate("chart.svg", clock.getTotalTime(), ord_tasks.size());
  task_info.displayFinalStatistics();
  gantt_chart.scrollChart();

  screen->erase();
  screen->refresh();
}