#include <locale.h>
#include <ncurses.h>
#include "core/system.hpp"
// #include "ui/screen.hpp"

using namespace Core;

#define CONFIG_FILE "configs/priop_maziero.txt"

int main()
{
    setlocale(LC_ALL, "pt_BR.UTF-8");

    System *system = System::getInstance();

    if (!system->loadConfig(CONFIG_FILE))
    {
        delete system;
        return 1;
    }

    flushinp();

    system->handleInterruption(Interruption::FULL_STOP);
    delete system;

    return 0;
}