#include "clock_mode.hpp"

#include "clock.hpp"

#include "../system.hpp"

using namespace Core;

ClockMode::ClockMode(Clock* c, System* sys)
    : clock(c)
    , system(sys)
{ }

ClockMode::~ClockMode()
{
    clock = nullptr;
    system = nullptr;
}

void ClockMode::checkModeChange()
{
    int ch = getch();

    if (ch == ' ')
        clock->selectMode('P');
}