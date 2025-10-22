#include "screen.hpp"
using namespace UI;

#include <ncurses.h>

Screen* Screen::instance(nullptr);

Screen::Screen()
{
    color_pair_count = 0;
    inverted = false;
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

void Screen::print(int x, int y, char ch)
{
    mvprintw(y, x, "%c", ch);
}

void Screen::print(int x, int y, string str)
{
    mvprintw(y, x, "%s", str.c_str());
}

void Screen::refresh()
{
    ::refresh();
}

void Screen::clear()
{
    ::clear();
}

void Screen::initColor(int color, int bg_color)
{
    init_pair(++color_pair_count, color, bg_color);
}

void Screen::setColor(DefaultColor color)
{
    attron(COLOR_PAIR(static_cast<int>(color)));
}

void Screen::setColor(int color_index)
{
    attron(COLOR_PAIR(color_index + 4));
}

void Screen::invertColor()
{
    if (inverted)
        attroff(A_REVERSE);
    else
        attron(A_REVERSE);

    inverted = !inverted;
}

void Screen::invertColor(bool inv)
{
    inverted = !inv;
    invertColor();
}

int Screen::getCh()
{
    return ::getch();
}