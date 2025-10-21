#pragma once
#include <string>
using namespace std;

namespace UI
{
    enum class DefaultColor
    {
        WHITE = 1,
        BLACK = 2,
        GRAY = 3
    };

    class Screen
    {
    private:
        static Screen* instance;
        int screen_edge_x;
        int screen_edge_y;
        int color_pair_count;
        bool inverted;
        Screen();
        void checkScreenEdges(int x, int y);

    public:
        ~Screen();
        static Screen* getInstance();
        int getScreenEdgeX() { return screen_edge_x; }
        int getScreenEdgeY() { return screen_edge_y; }
        void sprint(int x, int y, char ch = '*');
        void sprint(int x, int y, string str);
        void srefresh();
        void sclear();
        void initColor(int color, int bg_color);
        void setColor(DefaultColor color);
        void setColor(int color_index);
        void invertColor();
        void invertColor(bool inv);
        int sgetch();
    };
} // namespace UI