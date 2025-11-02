#pragma once

#include "../utils/std_libraries.hpp"

#include "tcb.hpp"

#include "scheduler.hpp"

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
