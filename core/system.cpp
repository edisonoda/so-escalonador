#include "system.hpp"

#include "clock.hpp"
#include "tcb.hpp"

using namespace Core;

System *System::instance(nullptr);

// IOEvent definition

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

// Mutex definition

Mutex::Mutex(System* sys, int id) : 
  system(sys),
  id(id)
{
  counter = 1;
  task = nullptr;
}

Mutex::~Mutex() {
  queue.clear();
  task = nullptr;
  system = nullptr;
}

void Mutex::lock(TCB* task) {
  if (counter == 0) {
    queue.push_back(task);
    system->handleInterruption(Interruption::MUTEX_LOCK);
  } else {
    this->task = task;
  }

  task->addMutex(this);
  counter = 0;
}

void Mutex::unlock() {
  if (task != nullptr) {
    task->removeMutex(this);
    task = nullptr;
  }

  if (queue.empty()) {
    counter = 1;
    return;
  }
  
  task = queue.front();
  queue.pop_front();
  system->handleInterruption(Interruption::MUTEX_UNLOCK, task);
}

int Mutex::getId() {
  return id;
}

// System Memento definition

// Realiza Deep Copy (clonagem) do estado para permitir Undo seguro
SystemMemento::SystemMemento(
  int clock_time,
  int clock_quantum,
  int task_count,
  bool was_random,
  vector<TCB*>& tasks,
  TCB* current_task,
  list<TCB*>& ready_list,
  list<TCB*>& suspended_list,
  list<TCB*>& new_list,
  list<IOEvent*>& ioevent_list,
  list<Mutex*>& mutex_list
) {
  this->clock_time = clock_time;
  this->clock_quantum = clock_quantum;
  this->task_count = task_count;
  this->was_random = was_random;

  // Mapeia ponteiros originais -> cópias para reconstruir referências
  map<TCB*, TCB*> ptr_map;
  map<IOEvent*, IOEvent*> ioevent_map;
  map<Mutex*, Mutex*> mutex_map;

  for (TCB* original : tasks) {
    TCB* copy = new TCB(*original);
    // Limpa eventos copiados por padrão para evitar Double Free na destruição
    copy->getEvents()->clear();
    copy->getMutexList()->clear();

    this->tasks.push_back(copy);
    ptr_map[original] = copy;
  }
  
  if (current_task != nullptr) {
    this->current_task = ptr_map[current_task];
  } else {
    this->current_task = nullptr;
  }

  for (IOEvent* e : ioevent_list) {
    IOEvent* copy = new IOEvent(*e);
    copy->setTask(ptr_map[e->getTask()]);
    this->ioevent_list.push_back(copy);
    ioevent_map[e] = copy;
  }
  
  for (Mutex* e : mutex_list) {
    Mutex* copy = new Mutex(*e);
    copy->setTask(ptr_map[e->getTask()]);

    copy->getTasks()->clear();
    for (TCB* t : *(e->getTasks()))
      copy->getTasks()->push_back(ptr_map[t]);

    this->mutex_list.push_back(copy);
    mutex_map[e] = copy;
  }

  for (TCB* original : tasks) {
    if (original->getCurrentEvent() != nullptr)
      ptr_map[original]->setCurrentEvent(ioevent_map[original->getCurrentEvent()]);
    
    for (Mutex* mutex : *(original->getMutexList()))
      ptr_map[original]->getMutexList()->push_back(new Mutex(*mutex));

    for (Event* event : *(original->getEvents()))
      ptr_map[original]->getEvents()->push_back(new Event(*event));
  }

  for (TCB* t : ready_list) {
    this->ready_list.push_back(ptr_map[t]);
  }

  for (TCB* t : suspended_list) {
    this->suspended_list.push_back(ptr_map[t]);
  }
  
  for (TCB* t : new_list) {
    this->new_list.push_back(ptr_map[t]);
  }
}

SystemMemento::~SystemMemento() {
  for (TCB* t : tasks) {
    delete t;
  }
  tasks.clear();

  for (IOEvent* e : ioevent_list) {
    delete e;
  }
  ioevent_list.clear();

  for (Mutex* e : mutex_list) {
    delete e;
  }
  mutex_list.clear();
}

// System definition

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

  for (IOEvent* ev : ioevent_list)
    delete ev;

  for (Mutex* m : mutex_list)
    delete m;

  for (SystemMemento* s : history)
    delete s;

  ord_tasks.clear();
  new_list.clear();
  ready_list.clear();
  suspended_list.clear();
  ioevent_list.clear();
  mutex_list.clear();
  history.clear();

  clock.detach(this);
}

System *System::getInstance() {
  if (instance == nullptr)
    instance = new System();

  return instance;
}

void System::tick() {
  // Reseta flag de sorteio a cada ciclo;
  scheduler->setRandomFlag(false);
  saveState(&history);
  
  if (current_task != nullptr)
    checkEvents();

  checkNewTasks();

  // Se não existe task em execução, busca uma task
  if (current_task == nullptr)
    changeState(TCBState::RUNNING);
  
  if (current_task != nullptr)
    checkEvents();
    
  // Se existe task em execução, mas o tempo restante de execução é 0, termina a task
  if (current_task != nullptr && current_task->getRemaining() <= 0)
    terminateTask();
}

void System::endTick() {
  // Se existe task em execução decrementa o tempo restante de execução da mesma
  if (current_task != nullptr)
    current_task->decrementRemaining(1);

  // Atualiza o gráfico e as informações das tarefas
  gantt_chart.drawTick(clock.getTotalTime());
  task_info.drawTick(clock.getTotalTime());

  saveState(&chart_history);
}

void System::handleInterruption(Interruption irq, TCB* task) {
  switch (irq) {
    case Interruption::QUANTUM:
      preemptTask(PreemptType::QUANTUM);
      break;
    case Interruption::FINISH_IO:
      if (task != nullptr)
        readyTask(task, EventType::IO);
      break;
    case Interruption::MUTEX_LOCK:
      suspendTask();
      break;
    case Interruption::MUTEX_UNLOCK:
      if (task != nullptr)
        readyTask(task, EventType::MU);
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

  if (current_task != nullptr && current_task->getState() == TCBState::SUSPENDED)
    current_task = nullptr;
  
  if (current_task != nullptr) {
    // Se o escalonador escolheu uma task, remove ela das lista da ready
    if (current_task->getState() == TCBState::READY) {
      bool is_ready = false;

      for (TCB* ready : ready_list)
        if (current_task == ready)
          is_ready = true;

      if (is_ready)
        ready_list.remove(current_task);
    }

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
  TCB* task = current_task;
  list<Event*>* events = task->getEvents();

  if (!events || events->empty())
    return;

  list<Event*>::iterator i = (*events).begin();
  int elapsed = task->getDuration() - task->getRemaining();

  while (i != (*events).end()) {
    if ((*i)->start <= elapsed) {
      switch ((*i)->type) {
        case EventType::IO: {
          if (task->getRemaining() <= 0)
            break;

          IOEvent* event = new IOEvent(task, this, &clock, (*i)->duration);
          ioevent_list.push_back(event);
          task->setCurrentEvent(event);
          suspendTask();
          break;
        }

        case EventType::MU: {
          for (auto m : mutex_list)
            if ((*i)->id == m->getId())
              m->unlock();
          break;
        }

        case EventType::ML: {
          bool found = false;
          for (auto m : mutex_list) {
            if ((*i)->id == m->getId()) {
              found = true;
              m->lock(task);
              break;
            }
          }

          if (found)
            break;
          
          Mutex* mutex = new Mutex(this, (*i)->id);
          mutex_list.push_back(mutex);
          mutex->lock(task);
          break;
        }

        default:
          break;
      }

      delete (*i);
      i = events->erase(i);
      
      if (task != current_task)
        return;
    } else {
      i++;
    }
  }
}

void System::terminateTask() {
  // Armazena o tempo em que a task foi terminada para o cálculo dos tempos médios
  current_task->setCompletionTime(clock.getTotalTime());

  list<Mutex*> unlock_queue;

  for (Mutex* m : *(current_task->getMutexList())) {
    if (m->getTask() == current_task)
      unlock_queue.push_back(m);
  }

  for (Mutex* m : unlock_queue)
    m->unlock();

  unlock_queue.clear();

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

  if (current_task == nullptr || current_task->getRemaining() > 0)
    changeState(TCBState::READY, type);
}

void System::readyTask(TCB* task, EventType type) {
  if (type == EventType::IO) {
    ioevent_list.remove(task->getCurrentEvent());
    clock.scheduleDeletion(task->getCurrentEvent());
    task->setCurrentEvent(nullptr);
  }

  if (task->isAvailable()) {
    suspended_list.remove(task);
    ready_list.push_back(task);
    task->setState(TCBState::READY);
  }

  preemptTask(PreemptType::NEW_TASK);
}

void System::saveState(vector<SystemMemento*>* history) {
  bool is_random = scheduler->getWasRandomChoice();
  SystemMemento* snap = new SystemMemento(
    clock.getTotalTime(),
    clock.getCurrentQ(),
    task_count,
    is_random,
    ord_tasks,
    current_task,
    ready_list,
    suspended_list,
    new_list,
    ioevent_list,
    mutex_list
  );
    
  history->push_back(snap);
}

void System::restoreState() {
  if (history.empty()) return;

  SystemMemento* snap = history.back();

  // Limpa memória do estado atual antes de carregar o backup
  for (TCB* t : ord_tasks) delete t;
  for (IOEvent* e : ioevent_list) delete e;
  for (Mutex* e : mutex_list) delete e;
  ord_tasks.clear();
  ready_list.clear();
  suspended_list.clear();
  new_list.clear();
  ioevent_list.clear();
  mutex_list.clear();

  // Restaura tempo com -1 para compensar incremento incondicional do loop Clock::run
  clock.setTotalTime(snap->clock_time - 1);

  // Restaura contador do Quantum ajustado (evita preempção incorreta ao retomar)
  clock.setCurrentQuantum(snap->clock_quantum - 1);
  task_count = snap->task_count;

  // Restaura flag de sorteio para manter consistência visual
  scheduler->setRandomFlag(snap->was_random);

  this->ord_tasks = snap->tasks;
  this->current_task = snap->current_task;
  this->ready_list = snap->ready_list;
  this->suspended_list = snap->suspended_list;
  this->new_list = snap->new_list;
  this->ioevent_list = snap->ioevent_list;
  this->mutex_list = snap->mutex_list;

  for (IOEvent* e : ioevent_list)
    clock.attach(e);

  task_info.drawTick(clock.getTotalTime());
  task_info.drawTick(snap->clock_time);
  gantt_chart.previousTick();

  restoreState(&history);
  restoreState(&chart_history);
}

void System::restoreState(vector<SystemMemento*>* history) {
  // Pega o último estado
  SystemMemento* snap = history->back();
  history->pop_back();

  // Evita que o destrutor do snapshot apague as tasks que agora são nossas
  snap->tasks.clear(); 
  snap->ioevent_list.clear();
  snap->mutex_list.clear();

  delete snap;
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
  scheduler->setAlpha(configs.alpha);

  ord_tasks = configs.tasks;
  new_list = list<TCB *>(begin(ord_tasks), end(ord_tasks));
  task_count = new_list.size();

  gantt_chart.setTasks(&ord_tasks);
  gantt_chart.setEvents(&ioevent_list, &mutex_list);
  task_info.setTasks(&ord_tasks);
  task_info.setEvents(&ioevent_list, &mutex_list);
  task_info.moveWindow(0, gantt_chart.getHeight());
  task_info.drawTick(0);
  
  clock.selectMode(configs.mode);
  clock.run();
}

void System::endProgram() {
  tick();
  endTick();
  
  clock.stop();
  gantt_exporter.generate("chart.svg", clock.getTotalTime(), ord_tasks.size(), &chart_history);
  task_info.displayFinalStatistics();
  gantt_chart.scrollChart();

  screen->erase();
  screen->refresh();
}