#pragma once

#include "../utils/std_libraries.hpp"

namespace UI
{
    enum class DefaultColor
    {
        WHITE = 1,
        BLACK = 2,
        GRAY = 3,
        GREEN = 4
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
        int setColor(DefaultColor color);
        int setColor(int color_index);
        void invertColor();
        void invertColor(bool inv);
    };
} // namespace UI