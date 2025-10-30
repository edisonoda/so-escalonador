#include "window.hpp"
#include <ncurses.h>

using namespace UI;

Window::Window()
    : window(newwin(0, 0, 0, 0))
    , screen(Screen::getInstance())
{
    inverted = false;
    keypad(window, true);
    screen->refresh();
}

Window::~Window()
{
    clear();
    delwin(window);
    screen->refresh();

    screen = nullptr;
    window = nullptr;
}

void Window::setWindowDimensions(int height, int width, int x, int y)
{
    this->height = height;
    this->width = width;
    this->x = x;
    this->y = y;

    wresize(window, height, width);
    mvwin(window, y, x);

    screen->refresh();
}

void Window::getPos(int* x, int* y)
{
    getyx(window, *y, *x);
}

int Window::getPosX()
{
    return getcurx(window);
}

int Window::getPosY()
{
    return getcury(window);
}

void Window::moveWindow(int x, int y)
{
    mvwin(window, y, x);
    screen->refresh();
}

void Window::move(int x, int y)
{
    wmove(window, y, x);
}

void Window::print(int ch)
{
    wprintw(window, "%c", ch);
}

void Window::print(string str)
{
    wprintw(window, "%s", str.c_str());
}

void Window::print(int x, int y, int ch)
{
    mvwprintw(window, y, x, "%c", ch);
}

void Window::print(int x, int y, string str)
{
    mvwprintw(window, y, x, "%s", str.c_str());
}

void Window::del(int x, int y)
{
    wmove(window, y, x);
    wdelch(window);
}

void Window::refresh()
{
    wrefresh(window);
}

void Window::clear()
{
    wclear(window);
}

int Window::getCh()
{
    return wgetch(window);
}

int Window::setColor(DefaultColor color)
{
    wattron(window, COLOR_PAIR(static_cast<int>(color)));
    return static_cast<int>(color);
}

int Window::setColor(int color_index)
{
    wattron(window, COLOR_PAIR(color_index + INITIAL_COLORS + 1));
    return color_index + INITIAL_COLORS + 1;
}

void Window::invertColor()
{
    if (inverted)
        wattroff(window, A_REVERSE);
    else
        wattron(window, A_REVERSE);

    inverted = !inverted;
}

void Window::invertColor(bool inv)
{
    inverted = !inv;
    invertColor();
}