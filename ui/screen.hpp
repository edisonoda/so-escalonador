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
        int color_pair_count;
        bool inverted;

        Screen();

    public:
        ~Screen();
        static Screen* getInstance();
        void print(int x, int y, char ch = '*');
        void print(int x, int y, string str);
        void refresh();
        void clear();
        void initColor(int color, int bg_color);
        void setColor(DefaultColor color);
        void setColor(int color_index);
        void invertColor();
        void invertColor(bool inv);
        int getCh();
    };
} // namespace UI