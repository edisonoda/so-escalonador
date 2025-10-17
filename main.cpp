#include "core/system.hpp"

#define CONFIG_FILE "configs/fifo1.txt"

int main() {
    Core::System* system = Core::System::getInstance();

    if (!system->loadConfig(CONFIG_FILE))
    {
        delete system;
        return 1;
    }
    
    delete system;
    return 0;
}