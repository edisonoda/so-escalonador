#include "scheduler.hpp"
#include <ncurses.h>

using namespace Core;

SchedulingAlgorithm::SchedulingAlgorithm(AlgorithmID id, list<TCB *> *task_list) : id(id), task_list(task_list), scheduler(Scheduler::getInstance()) {}

void SchedulingAlgorithm::setTaskList(list<TCB *> *task_list) {
  this->task_list = task_list;
}

TCB *SchedulingAlgorithm::tieBreaker(TCB* current_task, TCB* t1, TCB* t2) {
  if (t1 == t2) return t1;

  // Quem estava executando é escolhida
  if (t1 == current_task)
    return t1;
  if (t2 == current_task)
    return t2;

  // Quem iniciou primeiro é escolhida
  if (t1->getStart() < t2->getStart())
    return t1;
  if (t2->getStart() < t1->getStart())
    return t2;

  // Quem tem a menor duração é escolhida
  if (t1->getDuration() < t2->getDuration())
    return t1;
  if (t2->getDuration() < t1->getDuration())
    return t2;

  // Sorteio
  scheduler->setRandomFlag(true);
  return rand() % 2 ? t1 : t2;
}

// FIFO definition

FIFO::FIFO(list<TCB *> *task_list) : SchedulingAlgorithm(AlgorithmID::FIFO, task_list) {}

FIFO::~FIFO() {}

TCB *FIFO::chooseTask(TCB *current_task, PreemptType type) {
  // Se a lista não tiver mais tasks para executar retorna nulo
  if (task_list->empty())
    return nullptr;

  // Se ainda houver tasks e a preempção foi por tempo, retorna a task atual
  if (type == PreemptType::NEW_TASK)
    return current_task;

  // Se ainda tem tasks e a preempção não foi por tempo, retorna a que entrou primeiro
  return task_list->front();
}

// SRTF definition

SRTF::SRTF(list<TCB *> *task_list) : SchedulingAlgorithm(AlgorithmID::SRTF, task_list) {}

SRTF::~SRTF() {}

TCB *SRTF::chooseTask(TCB *current_task, PreemptType type) {
  // Apenas retorna a task atual caso ela não tenha terminado ainda
  if (task_list->empty()) {
    if (current_task == nullptr || current_task->getState() == TCBState::TERMINATED)
      return nullptr;

    return current_task;
  }

  TCB* chosen = current_task;
  // Se não tem uma task ativa ou a task ativa não estiver pronta, desconsidera a task atual
  if (current_task == nullptr || current_task->getState() != TCBState::READY)
    chosen = task_list->front();

  // Busca a tarefa com menor tempo restante
  for (TCB *task : *task_list) {
    if (task->getRemaining() < chosen->getRemaining())
      chosen = task;
    else if (task->getRemaining() == chosen->getRemaining())
      chosen = tieBreaker(current_task, chosen, task);
  }

  return chosen;
}

// PRIOp definition

PRIOp::PRIOp(list<TCB *> *task_list) : SchedulingAlgorithm(AlgorithmID::PRIOp, task_list) {}

PRIOp::~PRIOp() {}

TCB *PRIOp::chooseTask(TCB *current_task, PreemptType type) {
  // Apenas retorna a task atual caso ela não tenha terminado ainda 
  if (task_list->empty()) {
    if (current_task == nullptr || current_task->getState() == TCBState::TERMINATED)
      return nullptr;

    return current_task;
  }

  TCB* chosen = current_task;
  // Se não tem uma task ativa ou a task ativa não estiver pronta, desconsidera a task atual
  if (current_task == nullptr || current_task->getState() != TCBState::READY)
    chosen = task_list->front();

  // Busca a tarefa com maior prioridade
  for (TCB *task : *task_list) {
    if (task->getPriority() > chosen->getPriority())
      chosen = task;
    else if (task->getPriority() == chosen->getPriority())
      chosen = tieBreaker(current_task, chosen, task);
  }

  return chosen;
}

// PRIOPEnv definition

PRIOPEnv::PRIOPEnv(list<TCB *> *task_list) : SchedulingAlgorithm(AlgorithmID::PRIOPEnv, task_list) {}

PRIOPEnv::~PRIOPEnv() {}

TCB *PRIOPEnv::tieBreaker(TCB* current_task, TCB* t1, TCB* t2) {
  if (t1->getPriority() > t2->getPriority())
    return t1;
  if (t2->getPriority() > t1->getPriority())
    return t2;

  return SchedulingAlgorithm::tieBreaker(current_task, t1, t2);
}

TCB *PRIOPEnv::chooseTask(TCB *current_task, PreemptType type) {
  // Apenas retorna a task atual caso ela não tenha terminado ainda 
  if (task_list->empty()) {
    if (current_task == nullptr || current_task->getState() == TCBState::TERMINATED)
      return nullptr;

    return current_task;
  }

  if (type == PreemptType::QUANTUM)
    return current_task;

  if (type == PreemptType::NEW_TASK || (current_task != nullptr && current_task->getState() == TCBState::TERMINATED)) {
    for (TCB *task : *task_list)
      task->setPriorityD(task->getPriorityD() + scheduler->getAlpha());
  }

  TCB* chosen = current_task;
  // Se não tem uma task ativa ou a task ativa não estiver pronta, desconsidera a task atual
  if (current_task == nullptr || current_task->getState() != TCBState::READY)
    chosen = task_list->front();

  // Busca a tarefa com maior prioridade
  for (TCB *task : *task_list) {
    if (task->getPriorityD() > chosen->getPriorityD())
      chosen = task;
    else if (task->getPriorityD() == chosen->getPriorityD())
      chosen = tieBreaker(current_task, chosen, task);
  }

  chosen->setPriorityD(chosen->getPriority());

  return chosen;
}

// Scheduler definition

Scheduler *Scheduler::instance(nullptr);

Scheduler::Scheduler() : task_list(nullptr), algorithm(nullptr) {
  alpha = 0;
  choice_was_random = false;
}

Scheduler::~Scheduler() {
  delete algorithm;
  algorithm = nullptr;
  instance = nullptr;
  task_list = nullptr;
}

Scheduler *Scheduler::getInstance() {
  if (instance == nullptr)
    instance = new Scheduler();

  return instance;
}

int Scheduler::getAlpha() { return alpha; }

void Scheduler::setAlpha(int a) { alpha = a; }

void Scheduler::setAlgorithm(AlgorithmID id) {
  if (algorithm != nullptr)
    delete algorithm;

  switch (id) {
    case AlgorithmID::FIFO:
      algorithm = new FIFO(task_list);
      break;
    case AlgorithmID::SRTF:
      algorithm = new SRTF(task_list);
      break;
    case AlgorithmID::PRIOp:
      algorithm = new PRIOp(task_list);
      break;
    case AlgorithmID::PRIOPEnv:
      algorithm = new PRIOPEnv(task_list);
      break;
    default:
      algorithm = new FIFO(task_list);
      break;
  }
}

void Scheduler::setTaskList(list<TCB *> *task_list) {
  this->task_list = task_list;
  if (algorithm != nullptr)
    algorithm->setTaskList(task_list); 
}

TCB *Scheduler::chooseTask(TCB *current_task, PreemptType type) {
  return algorithm->chooseTask(current_task, type);
}