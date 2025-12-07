#include "core/system.hpp"
#include <ctime>

using namespace Core;

int main() {
    srand(time(NULL));
    setlocale(LC_ALL, "pt_BR.UTF-8");

    System *system = System::getInstance();

    system->loadConfig();

    delete system;

    endwin();

    return 0;
}