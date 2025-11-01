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
        if (mode->getTick())
        {
            total_time++;
            quantum++;

            system->tick(); // Antes o tick era feito depois do tratamento da interrupção
                            // porém isso fazia com que a task não tivesse o valor decrementado
                            // que acontece dentro do tick tipo o tick já aconteceu
                            // depois tem que tratar a interrupção

            if (quantum >= quantum_interval)
            {
                system->handleInterruption(Interruption::QUANTUM);
            }

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