#pragma once

#include "screen.hpp"
#include "../utils/std_libraries.hpp"

namespace UI
{
    class Window
    {
    protected:
        WINDOW* window;
        Screen* screen;

        int height;
        int width;
        int x;
        int y;

        bool inverted;

    public:
        Window();
        virtual ~Window();

        void setWindowDimensions(int height, int width, int x, int y);
        int getHeight() const { return height; }
        int getWidth() const { return width; }
        int getX() const { return x; }
        int getY() const { return y; }
        void moveWindow(int x, int y);

        void getPos(int* x, int* y);
        int getPosX();
        int getPosY();

        void move(int x, int y);
        void del(int x, int y);
        void refresh();
        void clear();
        void erase();
        int getCh();
        
        int setColor(DefaultColor color);
        int setColor(int color_index);
        void invertColor();
        void invertColor(bool inv);
        
        virtual void print(int ch);
        virtual void print(string str);
        virtual void print(int x, int y, int ch);
        virtual void print(int x, int y, string str);
    };
} // namespace UI