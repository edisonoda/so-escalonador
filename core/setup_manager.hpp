#pragma once

#include <vector>
#include <string>
#include "tcb.hpp"
#include "config_reader.hpp"
#include "../ui/setup_ui.hpp"
#include "../scheduler/scheduling_algorithm.hpp"

using namespace std;
using namespace Core;

struct SimulationConfig
{
    Scheduler::AlgorithmID alg_id = Scheduler::AlgorithmID::FCFS;
    int quantum = 0;
    vector<TCB*> tasks;
    bool simulation_should_run = false;
};

class SetupManager
{
private:
    ConfigReader config_reader;
    SetupUI* ui;

    Scheduler::AlgorithmID setup_alg_id;
    vector<TCB*> setup_tasks;
    int setup_quantum;

    bool loadFromFile(const string& filename);
    void runEditor();
    
    void addNewTask();
    void deleteTask();
    bool isNumber(const string &s);

public:
    SetupManager(UI::Screen* screen);
    ~SetupManager();

    SimulationConfig run();
};