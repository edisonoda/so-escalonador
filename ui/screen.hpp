#pragma once

namespace UI
{
    class Screen
    {
    private:
        static Screen* instance;
        Screen();
    
    public:
        ~Screen();
        static Screen* getInstance();
        void sprint(int x, int y, char ch = '*');
        void srefresh();
        void sclear();
        void setColor(int color);
        int sgetch();
    };
} // namespace UI