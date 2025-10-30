#include "utils/std_libraries.hpp"
#include "core/system.hpp"

using namespace Core;

int main()
{
    setlocale(LC_ALL, "pt_BR.UTF-8");

    System *system = System::getInstance();

    if (!system->loadConfig())
    {
        delete system;
        return 1;
    }

    timeout(-1);
    flushinp();

    system->handleInterruption(Interruption::FULL_STOP);
    delete system;

    return 0;
}