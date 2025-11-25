#include "core/system.hpp"

using namespace Core;

int main() {
    setlocale(LC_ALL, "pt_BR.UTF-8");

    System *system = System::getInstance();

    system->loadConfig();

    delete system;

    endwin();

    return 0;
}