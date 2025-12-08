#include "clock.hpp"

#include "system.hpp"

using namespace Core;

// ClockMode definition

ClockMode::ClockMode(Clock *c, System *sys) : clock(c), system(sys) {}

ClockMode::~ClockMode() {
  clock = nullptr;
  system = nullptr;
}

// Verifica se o usuário trocou de modo de execução, durante a simulação
void ClockMode::checkModeChange() {
  int ch = getch();

  if (ch == ' ')
    clock->selectMode('P');
}

// AutoClock definition

AutoClock::AutoClock(Clock *c, System *sys) : ClockMode(c, sys),
  tick_interval(DEFAULT_TICK_INTERVAL)
{
  tick_counter = 0;
  ticked = false;
  start_time = chrono::system_clock::now();
}

AutoClock::~AutoClock() {}

bool AutoClock::getTick() {
  // Faz cálculo do tempo decorrido desde a última verificação e converte em milisegundos
  end_time = chrono::system_clock::now();
  elapsed = end_time - start_time;
  int elap = (int)(elapsed.count() * 1000);

  ticked = false;

  // Se o tempo decorrido for maior ou igual ao tempo padrão de tick, o tick ocorre
  if (elap >= tick_interval && elap > 0) {
    ticked = true;

    // Calcula quantos ticks aconteceram de fato
    int t = (elap - (elap % tick_interval)) / tick_interval;
    tick_counter += t;

    // Reseta o contador de ticks, para evitar int overflow
    if (tick_counter >= 60)
      tick_counter = 0;

    start_time = end_time;
  }

  // Permite a pausa não bloqueante para trocar o modo para manual mesmo no modo automático
  timeout(0);
  int ch = getch();

  // Caso a troca tenha sido solicitada, seta o modo manual e cancela o tick
  if (ch == ' ') {
    clock->selectMode('P');
    ticked = false;
  }

  return ticked;
}

// ManualClock definition

ManualClock::ManualClock(Clock *c, System *sys) : ClockMode(c, sys) {}

ManualClock::~ManualClock() {}

bool ManualClock::getTick() {
  // Como está ocorrendo pausadamente espera pela entrada de troca de forma bloqueante
  timeout(-1);
  int ch = getch();

  if (ch == 'b') {
    system->restoreState();
    return false;
  }

  if (ch == 'q') {
    system->endProgram();
    return false;
  }

  if (ch == ' ') {
    clock->selectMode('A');
    return false;
  }

  return true;
}

// TickSubject definition

TickSubject::TickSubject() { }

TickSubject::~TickSubject() {
  observers.clear();
}

void TickSubject::attach(TickObserver* obs) {
  observers.insert(obs);
}

void TickSubject::detach(TickObserver* obs) {
  observers.erase(obs);
}

void TickSubject::scheduleDeletion(TickObserver* obs) {
  deletion_queue.push_back(obs);
}

void TickSubject::notify() {
  for (TickObserver* obs : observers)
    obs->tick();

  for (TickObserver* obs : deletion_queue) {
    observers.erase(obs);
    delete obs;
  }

  deletion_queue.clear();
}

// Clock definition

Clock::Clock(System *sys) : TickSubject(), system(sys), quantum_interval(DEFAULT_QUANTUM) {
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
      notify();

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