#include "clock.hpp"

#include "auto_clock.hpp"
#include "manual_clock.hpp"

#include "../system.hpp"
#include "../../utils/constants.hpp"

using namespace Core;

Clock::Clock(System *sys)
    : system(sys), quantum_interval(Constants::DEFAULT_QUANTUM)
{
    mode = nullptr;
    total_time = -1;
    quantum = 0;
    running = false;
}

Clock::~Clock()
{
    delete mode;
    mode = nullptr;
    system = nullptr;
}

void Clock::run()
{
    running = true;

    while (running)
    {
        // Chama o método virtual do modo de execução para verificar quando acontece um tick
        // -> Muda de acordo com a strategy
        if (mode->getTick()) 
        {
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

void Clock::stop()
{
    running = false;
}

void Clock::selectMode(char m)
{
    int total_time = 0;

    if (mode != nullptr)
        delete mode;

    // Seleção da strategy para o clock
    switch (m)
    {
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