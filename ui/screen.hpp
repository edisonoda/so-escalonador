#pragma once

#include <ncurses.h>
#include <string>
#include <set>

#define X_PAD 2
#define Y_PAD 1
#define INITIAL_COLORS 4

namespace UI
{
    enum class DefaultColor
    {
        WHITE = 1,
        BLACK = 2,
        GRAY = 3,
        GREEN = 4
    };

    class RefreshObserver
    {
        public:
            RefreshObserver() { };
            virtual ~RefreshObserver() { };

            virtual void update() = 0;
    };

    class RefreshSubject
    {
        private:
            std::set<RefreshObserver*> observers;

        public:
            RefreshSubject();
            ~RefreshSubject();

            void attach(RefreshObserver* obs);
            void detach(RefreshObserver* obs);
            void update();
    };

    class Screen : public RefreshSubject
    {
    private:
        static Screen* instance;
        int color_pair_count;

        Screen();

    public:
        ~Screen();
        static Screen* getInstance();
        
        void refresh();
        void erase();
        
        void initColor(int color, int bg_color);
    };

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
        virtual void print(std::string str);
        virtual void print(int x, int y, int ch);
        virtual void print(int x, int y, std::string str);
    };
} // namespace UI