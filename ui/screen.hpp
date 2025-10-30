#pragma once

#include "../utils/std_libraries.hpp"

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

    class Screen
    {
    private:
        static Screen* instance;
        int color_pair_count;

        Screen();

    public:
        ~Screen();
        static Screen* getInstance();
        
        void refresh();
        void clear();
        
        void initColor(int color, int bg_color);
    };
} // namespace UI