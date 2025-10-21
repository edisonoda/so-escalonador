#pragma once
#include <string>
using namespace std;

namespace UI
{
    class Screen
    {
    private:
        static Screen* instance;
        int screen_edge_x;
        int screen_edge_y;
        int color_pair_count;
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
        void setColor(int color_index);
        int sgetch();
    };
} // namespace UI