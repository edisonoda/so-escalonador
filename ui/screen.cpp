#include "screen.hpp"
#include <ncurses.h>

using namespace UI;

Screen *Screen::instance(nullptr);

int convertRGB(int color)
{
    return ((float)color / 255) * 1000;
}

Screen::Screen()
{
    color_pair_count = 0;

    initscr();
    noecho();
    start_color();
    init_color(100, 250, 250, 250);
    init_color(COLOR_BLACK, 100, 100, 125);
    init_color(COLOR_BLUE, convertRGB(66), convertRGB(135), convertRGB(245));
    init_color(COLOR_YELLOW, convertRGB(245), convertRGB(233), convertRGB(66));
    init_color(COLOR_CYAN, convertRGB(66), convertRGB(245), convertRGB(197));
    init_color(COLOR_RED, convertRGB(245), convertRGB(66), convertRGB(66));
    init_color(COLOR_MAGENTA, convertRGB(245), convertRGB(66), convertRGB(135));

    initColor(7, 0);           // branco no preto
    initColor(0, 0);           // preto no preto
    initColor(0, 100);         // preto no cinza
    initColor(COLOR_GREEN, 0); // verde no preto

    bkgd(COLOR_PAIR(1));
}

Screen::~Screen()
{
    instance = nullptr;
    endwin();
}

Screen *Screen::getInstance()
{
    if (instance == nullptr)
        instance = new Screen();

    return instance;
}

void Screen::refresh()
{
    ::refresh();
}

void Screen::erase()
{
    ::erase();
    update();
}

void Screen::initColor(int color, int bg_color)
{
    init_pair(++color_pair_count, color, bg_color);
}