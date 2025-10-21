#include "screen.hpp"
using namespace UI;

#include <ncurses.h>

Screen* Screen::instance(nullptr);

Screen::Screen()
{
    color_pair_count = 0;
    initscr();
    start_color();
    initColor(7, 0); // branco no preto
    initColor(0, 0); // preto no preto
    initColor(0, 8); // cinza no preto
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

void Screen::initColor(int color, int bg_color)
{
    init_pair(++color_pair_count, color, bg_color);
}

void Screen::setColor(int color_index)
{
    attron(COLOR_PAIR(color_index));
}

int Screen::sgetch()
{
    return ::getch();
}