#pragma once

#include "../utils/std_libraries.hpp"

#include "tcb.hpp"
#include "config_reader.hpp"

#include "../scheduler/scheduling_algorithm.hpp"

#include "../ui/setup_ui.hpp"
#include "../ui/screen.hpp"

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
        Screen* screen;
        SetupUI ui;

        SimulationConfig config;

        bool loadFromFile(const string& filename);
        void runEditor();
        void runTaskListEditor();
        void runAlgorithmEditor();
        
        void addNewTask();
        void deleteTask();
        void editTask(int index);
        bool isNumber(const string &s);

        bool validateEntry(string str);

    public:
        SetupManager();
        ~SetupManager();

        SimulationConfig run();
    };
} // namespace Core