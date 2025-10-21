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

void Screen::checkScreenEdges(int x, int y)
{
    if (x > screen_edge_x)
        screen_edge_x = x;

    if (y > screen_edge_y)
        screen_edge_y = y;
}

void Screen::sprint(int x, int y, char ch)
{
    checkScreenEdges(x, y);
    mvprintw(y, x, "%c", ch);
}

void Screen::sprint(int x, int y, string str)
{
    checkScreenEdges(x + str.length(), y);
    mvprintw(y, x, "%s", str.c_str());
}

void Screen::srefresh()
{
    ::refresh();
}

void Screen::sclear()
{
    ::clear();
}

void Screen::setColor(int color, int bg_color)
{
    init_pair(1, color, bg_color);
    attron(COLOR_PAIR(1));
}

int Screen::sgetch()
{
    return ::getch();
}