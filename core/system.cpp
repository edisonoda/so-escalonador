#include "system.hpp"

#include "../utils/constants.hpp"

#include <iostream>

using namespace Core;

System* System::instance(NULL);

System::System() : scheduler(Scheduler::Scheduler::getInstance()) {
    quantum = Constants::DEFAULT_QUANTUM;
    running = true;
    run();
}

System::~System()
{
    delete scheduler;
}

System* System::getInstance()
{
    if (instance != nullptr)
        return instance;

    instance = new System();
    return instance;
}

void System::run()
{
    while (running) {
        if (clock.getTick())
        {
            tick();
        }
    }
}

void System::tick()
{
    cout << "Ticked" << endl;
}

bool System::loadConfig(const string &filename)
{
    if (!config_reader.openFile(filename)) {
        printf("Failed to open config file.\n");
        return false;
    }

    config_reader.readPattern();
    quantum = config_reader.getQuantum();
    scheduler->setAlgorithm(config_reader.getAlgorithm().c_str());

    task_list = config_reader.readTasks();

    return true;
}