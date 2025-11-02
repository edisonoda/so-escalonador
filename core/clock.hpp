#pragma once

#include <chrono>

#define DEFAULT_QUANTUM 5.f
#define DEFAULT_TICK_INTERVAL 250 // Milisegundos

using namespace std;

// Forward declaration
namespace Core { class Clock; }
namespace Core { class System; }

namespace Core {
  class ClockMode {
    protected:
      Clock* clock;
      System* system;
      
      void checkModeChange();

    public:
      ClockMode(Clock* clock, System* sys);
      virtual ~ClockMode();

      virtual bool getTick() = 0;
  };

  class AutoClock : public ClockMode {
    private:
      const int tick_interval;
      int tick_counter;
      bool ticked;

      chrono::time_point<chrono::system_clock> start_time;
      chrono::time_point<chrono::system_clock> end_time;
      chrono::duration<double> elapsed;

    public:
      AutoClock(Clock* clock, System* sys);
      ~AutoClock();

      virtual bool getTick();
  };
  
  class ManualClock : public ClockMode {
    public:
      ManualClock(Clock* clock, System* sys);
      ~ManualClock();
      
      virtual bool getTick();
  };

  class Clock {
    private:
      System* system;
      ClockMode* mode;
      
      int total_time;
      int quantum_interval; // fixo
      int quantum; // dinâmico
      bool running;

    public:
      Clock(System* sys);
      virtual ~Clock();

      int getQuantum() const { return quantum_interval; }
      int getCurrentQ() const { return quantum; }
      void setQuantum(const int q) { quantum_interval = q; }
      void resetQuantum() { quantum = 0; }
      int getTotalTime() const { return total_time; }

      void selectMode(char mode);
      void run();
      void stop();
  };
}