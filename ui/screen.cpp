#include "screen.hpp"
using namespace UI;

#include <ncurses.h>

Screen* Screen::instance(nullptr);

Screen::Screen()
{
    initscr();
}

Screen::~Screen()
{
    instance = nullptr;
    endwin();
}

Screen* Screen::getInstance()
{
    if (instance != nullptr)
        return instance;

    instance = new Screen();
    return instance;
}