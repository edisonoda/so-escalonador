#include "utils/std_libraries.hpp"
#include "core/system.hpp"

using namespace Core;

int main()
{
    setlocale(LC_ALL, "pt_BR.UTF-8");
    clear_log();

    System *system = System::getInstance();

    system->loadConfig();

    delete system;

    return 0;
}