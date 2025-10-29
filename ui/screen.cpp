#include "screen.hpp"
#include <ncurses.h>

#define INITIAL_COLORS 4

using namespace UI;

Screen* Screen::instance(nullptr);

Screen::Screen()
{
    color_pair_count = 0;
    inverted = false;
    initscr();
    start_color();
    init_color(100, 250, 250, 250);

    initColor(7, 0); // branco no preto
    initColor(0, 0); // preto no preto
    initColor(0, 100); // preto no cinza
    initColor(COLOR_GREEN, 0); // verde no preto

    bkgd(COLOR_PAIR(1));
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

int Screen::setColor(DefaultColor color)
{
    attron(COLOR_PAIR(static_cast<int>(color)));
    return static_cast<int>(color);
}

int Screen::setColor(int color_index)
{
    attron(COLOR_PAIR(color_index + INITIAL_COLORS + 1));
    return color_index + INITIAL_COLORS + 1;
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

char Screen::getCh()
{
    return ::getch();
}