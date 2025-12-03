#pragma once

#include "tcb.hpp"
#include "clock.hpp"
#include "setup_manager.hpp"

#include "scheduler.hpp"

#include "../ui/screen.hpp"
#include "../ui/task_visual.hpp"

namespace Core {
  enum class Interruption {
    QUANTUM,
    FINISH_IO,
    MUTEX_LOCK,
    MUTEX_UNLOCK,
    FULL_STOP
  };

  class IOEvent : public TickObserver {
    private:
      const int duration;
      int remaining_time;

      TCB* task;
      System* system;
      Clock* clock;
    
    public:
      IOEvent(TCB* task, System* system, Clock* clock, const int duration);
      ~IOEvent();

      virtual void tick();
  };

  class Mutex {
    private:
      int id;
      int counter;

      System* system;
      list<TCB*> queue;
    
    public:
      Mutex(System* system, int id);
      ~Mutex();

      void lock(TCB* task);
      void unlock();
      int getId();
  };

  class System : public TickObserver {
    private:
      static System* instance;
      
      Scheduler* scheduler;
      Clock clock;
      TCB* current_task;
      vector<TCB*> ord_tasks;
      list<TCB*> new_list;
      list<TCB*> ready_list;
      list<TCB*> suspended_list;
      list<IOEvent*> ioevent_list;
      list<Mutex*> mutex_list;

      Screen* screen;
      GanttChart gantt_chart;
      TaskInfo task_info;

      GanttExporter gantt_exporter;
      SetupManager setup;

      int task_count;

      // Singleton
      System();

      void changeState(TCBState state, PreemptType type = PreemptType::NONE);
      void checkNewTasks();
      void checkEvents();
      void terminateTask();
      void suspendTask();
      void readyTask(TCB* task, EventType type);
      void preemptTask(PreemptType type);
  
      void endProgram();
      
    public:
      ~System();
      static System* getInstance();

      void loadConfig();
      void handleInterruption(Interruption irq, TCB* task = nullptr);  
      void endTick();
      virtual void tick();
  };
} // namespace Core