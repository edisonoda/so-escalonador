#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include "tcb.hpp"
#include "../scheduler/scheduling_algorithm.hpp"

using namespace std;
using namespace Scheduler;

namespace Core
{
    class ConfigReader
    {
    private:
        ifstream file;
        string algorithm;
        int quantum;
        const map<string, AlgorithmID> alg_map;

        string readLine();
    
    public:
        ConfigReader();
        ~ConfigReader();

        bool openFile(const string &filename);
        void closeFile();
        void readPattern();
        AlgorithmID getAlgorithm() const;
        int getQuantum() const { return quantum; }
        list<TCB*> readTasks();
    };
} // namespace Core
