#include "clock.hpp"

#include "system.hpp"

using namespace Core;

ClockMode::ClockMode(Clock *c, System *sys) : clock(c), system(sys) {}

ClockMode::~ClockMode() {
  clock = nullptr;
  system = nullptr;
}

void ClockMode::checkModeChange() {
  int ch = getch();

  if (ch == ' ')
    clock->selectMode('P');
}

AutoClock::AutoClock(Clock *c, System *sys)
    : ClockMode(c, sys), tick_interval(DEFAULT_TICK_INTERVAL) {
  tick_counter = 0;
  ticked = false;
  start_time = std::chrono::system_clock::now();
}

AutoClock::~AutoClock() {}

bool AutoClock::getTick() {
  end_time = std::chrono::system_clock::now();
  elapsed = end_time - start_time;
  int elap = (int)(elapsed.count() * 1000);

  ticked = false;

  if (elap >= tick_interval && elap > 0) {
    ticked = true;

    int t = (elap - (elap % tick_interval)) / tick_interval;
    tick_counter += t;

    if (tick_counter >= 60)
      tick_counter = 0;

    start_time = end_time;
  }

  timeout(0);
  int ch = getch();

  if (ch == ' ') {
    clock->selectMode('P');
    ticked = false;
  }

  return ticked;
}

ManualClock::ManualClock(Clock *c, System *sys) : ClockMode(c, sys) {}

ManualClock::~ManualClock() {}

bool ManualClock::getTick() {
  timeout(-1);
  int ch = getch();

  if (ch == ' ') {
    clock->selectMode('A');
    return false;
  }

  return true;
}

Clock::Clock(System *sys) : system(sys), quantum_interval(DEFAULT_QUANTUM) {
  mode = nullptr;
  total_time = -1;
  quantum = 0;
  running = false;
}

Clock::~Clock() {
  delete mode;
  mode = nullptr;
  system = nullptr;
}

void Clock::run() {
  running = true;

  while (running) {
    // Chama o método virtual do modo de execução para verificar quando acontece
    // um tick
    // -> Muda de acordo com a strategy
    if (mode->getTick()) {
      total_time++;
      quantum++;

      // Começa a execução do tick
      system->startTick();

      // Verifica o quantum e executa a preempção por tempo
      if (quantum >= quantum_interval)
        system->handleInterruption(Interruption::QUANTUM);

      // Encerra a execução do tick
      system->endTick();
    }
  }
}

void Clock::stop() { running = false; }

void Clock::selectMode(char m) {
  int total_time = 0;

  if (mode != nullptr)
    delete mode;

  // Seleção da strategy para o clock
  switch (m) {
  case 'A':
    mode = new AutoClock(this, system);
    break;
  case 'P':
    mode = new ManualClock(this, system);
    break;
  default:
    mode = new AutoClock(this, system);
    break;
  }
}