#include <stdio.h>
#include <list>
#include "core/config_reader.hpp"
#include "core/tcb.hpp"

int main() {
    Core::ConfigReader reader("configs/fifo1.txt");
    if (!reader.openFile()) {
        printf("Failed to open config file.\n");
        return 1;
    }

    reader.readPattern();
    printf("Algorithm: %s\n", reader.getAlgorithm().c_str());
    printf("Quantum: %d\n", reader.getQuantum());

    list<Core::TCB*> tasks = reader.readTasks();
    for (Core::TCB* task : tasks)
    {
        printf("Task ID: %s\n", task->getId().c_str());
        delete task;
    }

    return 0;
}