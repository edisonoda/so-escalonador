#include "core/system.hpp"
// #include "ui/screen.hpp"

using namespace Core;

#define CONFIG_FILE "configs/fcfs_maziero.txt"

int main()
{
    System *system = System::getInstance();

    if (!system->loadConfig(CONFIG_FILE))
    {
        delete system;
        return 1;
    }

    delete system;

    return 0;
}