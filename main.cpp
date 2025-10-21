#include "core/system.hpp"
// #include "ui/screen.hpp"

#define CONFIG_FILE "configs/fifo1.txt"

int main() {
    Core::System* system = Core::System::getInstance();

    if (!system->loadConfig(CONFIG_FILE))
    {
        delete system;
        return 1;
    }
    
    delete system;

    // UI::Screen* screen = UI::Screen::getInstance();
    // screen->setColor(2);
    // screen->sprint(10, 5, 'A');
    // screen->srefresh();
    // screen->sgetch();
    // delete screen;

    return 0;
}