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
        void setColor(int color, int bg_color);
        int sgetch();
    };
} // namespace UI