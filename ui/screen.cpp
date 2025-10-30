#include "screen.hpp"
#include <ncurses.h>

using namespace UI;

Screen* Screen::instance(nullptr);

Screen::Screen()
{
    color_pair_count = 0;

    initscr();
    noecho();
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
    if (instance == nullptr)
        instance = new Screen();

    return instance;
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