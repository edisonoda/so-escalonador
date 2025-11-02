#include "scheduler.hpp"

using namespace Core;

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

  // Se não tem uma task ativa ou a task ativa não estiver pronta, desconsidera a task atual
  if (current_task == nullptr || current_task->getState() != TCBState::READY)
    current_task = task_list->front();

  // Busca a tarefa com menor tempo restante
  for (TCB *task : *task_list) {
    if (task->getRemaining() < current_task->getRemaining())
      current_task = task;
  }

  return current_task;
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

  // Se não tem uma task ativa ou a task ativa não estiver pronta, desconsidera a task atual
  if (current_task == nullptr || current_task->getState() != TCBState::READY)
    current_task = task_list->front();

  // Busca a tarefa com maior prioridade
  for (TCB *task : *task_list) {
    if (task->getPriority() > current_task->getPriority())
      current_task = task;
  }

  return current_task;
}

// Scheduler definition

Scheduler *Scheduler::instance(nullptr);

Scheduler::Scheduler() : task_list(nullptr), algorithm(nullptr) {}

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
    default:
      algorithm = new FIFO(task_list);
      break;
  }
}

void Scheduler::setTaskList(list<TCB *> *task_list) {
  this->task_list = task_list;
}

TCB *Scheduler::chooseTask(TCB *current_task, PreemptType type) {
  return algorithm->chooseTask(current_task, type);
}