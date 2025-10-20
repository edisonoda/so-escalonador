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

    };
} // namespace Screen