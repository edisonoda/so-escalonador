#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include "tcb.hpp"
#include "../scheduler/scheduling_algorithm.hpp"

using namespace std;

namespace Core
{
    class ConfigReader
    {
    private:
        ifstream file;
        string algorithm;
        int quantum;
        const map<string, Scheduler::AlgorithmID> alg_map;

        string readLine();
    
    public:
        ConfigReader();
        ~ConfigReader();

        bool openFile(const string &filename);
        void closeFile();
        void readPattern();
        Scheduler::AlgorithmID getAlgorithm() const;
        int getQuantum() const { return quantum; }
        list<TCB*> readTasks();
    };
} // namespace Core
