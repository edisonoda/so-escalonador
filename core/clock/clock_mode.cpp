#include "clock_mode.hpp"
#include "clock.hpp"
#include "../system.hpp"

#include <ncurses.h>

using namespace Core;

ClockMode::ClockMode(Clock* c, System* sys)
    : clock(c)
    , system(sys)
    , screen(UI::Screen::getInstance())
{ }

ClockMode::~ClockMode()
{
    clock = nullptr;
    system = nullptr;
    screen = nullptr;
}

void ClockMode::checkModeChange()
{
    char ch = getch();

    if (ch == ' ')
        clock->selectMode('P');
}