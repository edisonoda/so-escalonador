#pragma once

#include "../core/tcb.hpp"

namespace Core {
  enum class AlgorithmID {
    FIFO,
    SRTF,
    PRIOp
  };

  enum class PreemptType {
    QUANTUM,
    NEW_TASK,
    NONE
  };

  class SchedulingAlgorithm {
    protected:
      const AlgorithmID id;
      list<TCB *> *task_list;

    public:
      SchedulingAlgorithm(AlgorithmID id, list<TCB *> *task_list) : id(id), task_list(task_list) {};
      virtual ~SchedulingAlgorithm() { task_list = nullptr; }

      AlgorithmID getID() const { return id; }

      virtual TCB *chooseTask(TCB *current_task = nullptr, PreemptType type = PreemptType::NONE) = 0;
  };

  class FIFO : public SchedulingAlgorithm {
    public:
      FIFO(list<TCB*>* task_list);
      ~FIFO();
      virtual TCB* chooseTask(TCB* current_task = nullptr, PreemptType type = PreemptType::NONE);
  };

  class SRTF : public SchedulingAlgorithm {
    public:
      SRTF(list<TCB*>* task_list);
      ~SRTF();
      virtual TCB* chooseTask(TCB* current_task = nullptr, PreemptType type = PreemptType::NONE);
  };

  class PRIOp : public SchedulingAlgorithm {
    public:
      PRIOp(list<TCB*>* task_list);
      ~PRIOp();
      virtual TCB* chooseTask(TCB* current_task = nullptr, PreemptType type = PreemptType::NONE);
  };

  class Scheduler {
    private:
      static Scheduler *instance;

      list<TCB *> *task_list;
      SchedulingAlgorithm *algorithm;

      // Singleton
      Scheduler();

    public:
      ~Scheduler();
      static Scheduler *getInstance();

      void setAlgorithm(AlgorithmID id);
      void setTaskList(list<TCB *> *task_list);
      TCB *chooseTask(TCB *current_task = nullptr, PreemptType type = PreemptType::NONE);
  };
} // namespace Scheduler