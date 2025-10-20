#include "screen.hpp"
using namespace UI;

#include <ncurses.h>

Screen* Screen::instance(nullptr);

Screen::Screen()
{
    initscr();
    start_color();
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

void Screen::sprint(int x, int y, char ch)
{
    mvprintw(y, x, "%c", ch);
}

void Screen::srefresh()
{
    ::refresh();
}

void Screen::sclear()
{
    ::clear();
}

void Screen::setColor(int color)
{
    init_pair(1, color, COLOR_BLACK);
    attron(COLOR_PAIR(1));
}

int Screen::sgetch()
{
    return ::getch();
}