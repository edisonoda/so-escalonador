#pragma once

#include <vector>
#include <string>
#include "tcb.hpp"
#include "config_reader.hpp"
#include "../ui/setup_ui.hpp"
#include "../scheduler/scheduling_algorithm.hpp"

using namespace std;
using namespace UI;

namespace Core
{
    struct SimulationConfig
    {
        Scheduler::AlgorithmID alg_id = Scheduler::AlgorithmID::FCFS;
        vector<TCB*> tasks;

        int quantum = 2;
        char mode = 'A';
        bool simulation_should_run = false;
    };

    class SetupManager
    {
    private:
        ConfigReader config_reader;
        SetupUI ui;

        SimulationConfig config;

        bool loadFromFile(const string& filename);
        void runEditor();
        
        void addNewTask();
        void deleteTask();
        bool isNumber(const string &s);

    public:
        SetupManager();
        ~SetupManager();

        SimulationConfig run();
    };
} // namespace Core